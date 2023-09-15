package main

// typedef void (*tailscale_send_file_callback)(unsigned long);
// extern void tailscale_bridge_send_file_callback(tailscale_send_file_callback cb, unsigned long bytes_sent);
import "C"

import (
	"context"
	"errors"
	"fmt"
	"io"
	"net"
	"net/netip"
	"os"
	"path/filepath"
	"strings"
	"sync"
	"sync/atomic"
	"time"

	"golang.org/x/net/idna"
	"tailscale.com/cmd/tailscale/cli"
	"tailscale.com/ipn/ipnstate"
	"tailscale.com/net/tsaddr"
	"tailscale.com/tailcfg"
	"tailscale.com/util/dnsname"
)

//export tailscale_receive_files
func tailscale_receive_files(strategy string, directory string) bool {
	args := []string{"file", "get", "-loop=true", "-verbose=true", "-wait=true", "-conflict", strategy, directory}
	if err := cli.Run(args); err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	return true
}

func getTargetStableID(ctx context.Context, ipStr string) (id tailcfg.StableNodeID, isOffline bool, err error) {
	ip, err := netip.ParseAddr(ipStr)
	if err != nil {
		return "", false, err
	}
	fts, err := client.FileTargets(ctx)
	if err != nil {
		return "", false, err
	}
	for _, ft := range fts {
		n := ft.Node
		for _, a := range n.Addresses {
			if a.Addr() != ip {
				continue
			}
			isOffline = n.Online != nil && !*n.Online
			return n.StableID, isOffline, nil
		}
	}

	found := false
	if st, err := client.Status(ctx); err == nil && st.Self != nil {
		for _, peer := range st.Peer {
			for _, pip := range peer.TailscaleIPs {
				if pip == ip {
					found = true
					if peer.UserID != st.Self.UserID {
						return "", false, errors.New("can only send files to your own devices")
					}
				}
			}
		}
	}
	if found {
		return "", false, errors.New("target seems to be running an old Tailscale version")
	}
	if !tsaddr.IsTailscaleIP(ip) {
		return "", false, fmt.Errorf("unknown target; %v is not a Tailscale IP address", ip)
	}
	return "", false, errors.New("unknown target")
}

func dnsOrQuoteHostname(st *ipnstate.Status, ps *ipnstate.PeerStatus) string {
	baseName := dnsname.TrimSuffix(ps.DNSName, st.MagicDNSSuffix)
	if baseName != "" {
		if strings.HasPrefix(baseName, "xn-") {
			if u, err := idna.ToUnicode(baseName); err == nil {
				return fmt.Sprintf("%s (%s)", baseName, u)
			}
		}
		return baseName
	}
	return fmt.Sprintf("(%q)", dnsname.SanitizeHostname(ps.HostName))
}

func tailscaleIPFromArg(ctx context.Context, hostOrIP string) (ip string, self bool, err error) {
	// If the argument is an IP address, use it directly without any resolution.
	if net.ParseIP(hostOrIP) != nil {
		return hostOrIP, false, nil
	}

	// Otherwise, try to resolve it first from the network peer list.
	st, err := client.Status(ctx)
	if err != nil {
		return "", false, err
	}
	match := func(ps *ipnstate.PeerStatus) bool {
		return strings.EqualFold(hostOrIP, dnsOrQuoteHostname(st, ps)) || hostOrIP == ps.DNSName
	}
	for _, ps := range st.Peer {
		if match(ps) {
			if len(ps.TailscaleIPs) == 0 {
				return "", false, errors.New("node found but lacks an IP")
			}
			return ps.TailscaleIPs[0].String(), false, nil
		}
	}
	if match(st.Self) && len(st.Self.TailscaleIPs) > 0 {
		return st.Self.TailscaleIPs[0].String(), true, nil
	}

	// Finally, use DNS.
	var res net.Resolver
	if addrs, err := res.LookupHost(ctx, hostOrIP); err != nil {
		return "", false, fmt.Errorf("error looking up IP of %q: %v", hostOrIP, err)
	} else if len(addrs) == 0 {
		return "", false, fmt.Errorf("no IPs found for %q", hostOrIP)
	} else {
		return addrs[0], false, nil
	}
}

type CountingReader struct {
	io.Reader
	bytesRead atomic.Uint64
}

func (reader *CountingReader) Read(buf []byte) (int, error) {
	bytesRead, err := reader.Reader.Read(buf)
	reader.bytesRead.Add(uint64(bytesRead))
	return bytesRead, err
}

func tailscale_send_report_progress(wg *sync.WaitGroup, done <-chan struct{}, reader *CountingReader, cb C.tailscale_send_file_callback) {
	defer wg.Done()

	for {
		select {
		case <-done:
			return
		case <-time.After(time.Second):
			lastBytesRead := reader.bytesRead.Load()
			C.tailscale_bridge_send_file_callback(cb, C.ulong(lastBytesRead))
		}
	}
}

//export tailscale_send_file
func tailscale_send_file(target string, file string, cb C.tailscale_send_file_callback) bool {

	hadBrackets := false
	if strings.HasPrefix(target, "[") && strings.HasSuffix(target, "]") {
		target = strings.TrimSuffix(strings.TrimPrefix(target, "["), "]")
		hadBrackets = true
	}
	if ip, err := netip.ParseAddr(target); err == nil && ip.Is6() && !hadBrackets {
		fmt.Fprintf(os.Stderr, "an IPv6 literal must be written as [%s]", ip)
		return false
	} else if hadBrackets && (err != nil || !ip.Is6()) {
		fmt.Fprintln(os.Stderr, "unexpected brackets around target")
		return false
	}
	ip, _, err := tailscaleIPFromArg(context.Background(), target)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}

	stableID, isOffline, err := getTargetStableID(context.Background(), ip)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	if isOffline {
		fmt.Fprintln(os.Stderr, "target is offline")
		return false
	}

	f, err := os.Open(file)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	defer f.Close()

	stat, err := f.Stat()
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}

	if stat.IsDir() {
		fmt.Fprintln(os.Stderr, "cannot send a directory")
		return false
	}

	contentLength := stat.Size()
	fileContents := CountingReader{io.LimitReader(f, contentLength), atomic.Uint64{}}

	done := make(chan struct{}, 1)
	wg := sync.WaitGroup{}

	go tailscale_send_report_progress(&wg, done, &fileContents, cb)
	wg.Add(1)

	err = client.PushFile(context.Background(), stableID, contentLength, filepath.Base(file), &fileContents)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	done <- struct{}{}
	wg.Wait()

	return true
}
