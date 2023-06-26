package main

import "C"

import (
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"net"
	"net/netip"
	"os"
	"path/filepath"
	"strconv"
	"strings"

	"golang.org/x/net/idna"
	"tailscale.com/client/tailscale"
	"tailscale.com/cmd/tailscale/cli"
	"tailscale.com/ipn/ipnstate"
	"tailscale.com/net/interfaces"
	"tailscale.com/net/tsaddr"
	"tailscale.com/tailcfg"
	"tailscale.com/util/dnsname"
)

var client tailscale.LocalClient

//export tailscale_down
func tailscale_down() {
	args := []string{"down"}
	if err := cli.Run(args); err != nil {
		fmt.Fprintln(os.Stderr, err)
	}
}

//export tailscale_up
func tailscale_up() {
	args := []string{"up"}
	if err := cli.Run(args); err != nil {
		fmt.Fprintln(os.Stderr, err)
	}
}

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

//export tailscale_send_file
func tailscale_send_file(target string, file string) bool {

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
	fileContents := io.LimitReader(f, contentLength)

	err = client.PushFile(context.Background(), stableID, contentLength, filepath.Base(file), fileContents)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}

	return true
}

//export tailscale_status
func tailscale_status(status_json *string) bool {
	status, err := client.Status(context.Background())
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	j, err := json.MarshalIndent(status, "", "  ")
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	*status_json = string(j)
	return true
}

//export tailscale_get_interface_name
func tailscale_get_interface_name(name *string) bool {
	_, iface, err := interfaces.Tailscale()
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		*name = ""
		return false
	}
	if iface == nil {
		fmt.Fprintln(os.Stderr, "no tailscale interface found")
		*name = ""
		return false
	}
	*name = iface.Name
	return true
}

//export tailscale_get_accept_routes
func tailscale_get_accept_routes(accept_routes *bool) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	*accept_routes = curPrefs.RouteAll
	return true
}

//export tailscale_set_accept_routes
func tailscale_set_accept_routes(accept_routes *bool) bool {
	args := []string{"set", "--accept-routes=" + strconv.FormatBool(*accept_routes)}
	err := cli.Run(args)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	return true
}

//export tailscale_get_advertise_exit_node
func tailscale_get_advertise_exit_node(exit_node *bool) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	*exit_node = curPrefs.AdvertisesExitNode()
	return true
}

//export tailscale_set_advertise_exit_node
func tailscale_set_advertise_exit_node(exit_node *bool) bool {
	args := []string{"set", "--advertise-exit-node=" + strconv.FormatBool(*exit_node)}
	if err := cli.Run(args); err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	return true
}

//export tailscale_get_accept_dns
func tailscale_get_accept_dns(accept_dns *bool) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	*accept_dns = curPrefs.CorpDNS
	return true
}

//export tailscale_set_accept_dns
func tailscale_set_accept_dns(accept_dns *bool) bool {
	args := []string{"set", "--accept-dns=" + strconv.FormatBool(*accept_dns)}
	if err := cli.Run(args); err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	return true
}

//export tailscale_get_hostname
func tailscale_get_hostname(hostname *string) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	*hostname = curPrefs.Hostname
	return true
}

//export tailscale_set_hostname
func tailscale_set_hostname(hostname *string) bool {
	args := []string{"set", "--hostname=" + *hostname}
	if err := cli.Run(args); err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	return true
}

//export tailscale_get_operator_user
func tailscale_get_operator_user(user *string) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	*user = curPrefs.OperatorUser
	return true
}

//export tailscale_set_operator_user
func tailscale_set_operator_user(user *string) bool {
	args := []string{"set", "--operator=" + *user}
	if err := cli.Run(args); err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	return true
}

//export tailscale_get_shields_up
func tailscale_get_shields_up(shields_up *bool) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	*shields_up = curPrefs.ShieldsUp
	return true
}

//export tailscale_set_shields_up
func tailscale_set_shields_up(shields_up *bool) bool {
	args := []string{"set", "--shields-up=" + strconv.FormatBool(*shields_up)}
	if err := cli.Run(args); err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	return true
}

//export tailscale_get_ssh
func tailscale_get_ssh(ssh *bool) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	*ssh = curPrefs.RunSSH
	return true
}

//export tailscale_set_ssh
func tailscale_set_ssh(ssh *bool) bool {
	args := []string{"set", "--ssh=" + strconv.FormatBool(*ssh)}
	if err := cli.Run(args); err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	return true
}

func main() {
}
