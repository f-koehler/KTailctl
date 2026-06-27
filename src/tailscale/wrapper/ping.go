package main

import "C"

import (
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"net/netip"
	"time"

	"tailscale.com/client/local"
	"tailscale.com/tailcfg"
)

// Flattened representation of a single Tailscale ping.
//
// It is serialized to JSON and parsed on the C++ side (see Pinger). Unlike ipnstate.PingResult it always reports an explicit success flag and a human-readable "via" path, and folds failures (invalid address, timeout, ...) into the same Error field.
type pingResult struct {
	Success   bool    `json:"success"`
	LatencyMs float64 `json:"latencyMs"`
	Via       string  `json:"via"`
	NodeName  string  `json:"nodeName"`
	NodeIP    string  `json:"nodeIP"`
	Error     string  `json:"error"`
}

func pingResultJSON(result pingResult) *C.char {
	j, err := json.Marshal(result)
	if err != nil {
		log_critical(fmt.Sprintf("failed to serialize ping result: %v", err))
		return nil
	}
	return C.CString(string(j))
}

func pingErrorJSON(msg string) *C.char {
	return pingResultJSON(pingResult{Error: msg})
}

// tailscale_ping performs a single Tailscale ping to the given address and returns the outcome as JSON. The continuous ping loop and any scheduling/pausing is driven from the C++ side, which calls this once per sample. The call blocks for at most timeoutSeconds.
//
//export tailscale_ping
func tailscale_ping(address *string, timeoutSeconds float64) *C.char {
	if address == nil || *address == "" {
		return pingErrorJSON("no address given")
	}

	ip, err := netip.ParseAddr(*address)
	if err != nil {
		return pingErrorJSON(fmt.Sprintf("invalid address %q: %v", *address, err))
	}

	timeout := time.Duration(timeoutSeconds * float64(time.Second))
	if timeout <= 0 {
		timeout = 5 * time.Second
	}

	ctx, cancel := context.WithTimeout(context.Background(), timeout)
	defer cancel()

	pr, err := client.PingWithOpts(ctx, ip, tailcfg.PingDisco, local.PingOpts{})
	if err != nil {
		if errors.Is(err, context.DeadlineExceeded) {
			return pingErrorJSON("timeout")
		}
		return pingErrorJSON(err.Error())
	}
	if pr.Err != "" {
		return pingErrorJSON(pr.Err)
	}

	var via string
	switch {
	case pr.PeerRelay != "":
		via = fmt.Sprintf("peer-relay(%s)", pr.PeerRelay)
	case pr.DERPRegionID != 0:
		via = fmt.Sprintf("DERP(%s)", pr.DERPRegionCode)
	case pr.Endpoint != "":
		via = "direct"
	default:
		via = string(tailcfg.PingDisco)
	}

	return pingResultJSON(pingResult{
		Success:   true,
		LatencyMs: pr.LatencySeconds * 1000.0,
		Via:       via,
		NodeName:  pr.NodeName,
		NodeIP:    pr.NodeIP,
	})
}
