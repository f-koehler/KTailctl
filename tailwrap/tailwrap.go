package main

import "C"

import (
	"context"
	"encoding/json"
	"fmt"

	"tailscale.com/client/tailscale"
	"tailscale.com/cmd/tailscale/cli"
	"tailscale.com/ipn"
)

var client tailscale.LocalClient

//export tailscale_down
func tailscale_down() {
	ctx := context.Background()
	status, err := client.Status(ctx)
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale status: %v", err))
		return
	}
	if status.BackendState == "Stopped" {
		log_warning("tailscale already down")
		return
	}

	_, err = client.EditPrefs(ctx, &ipn.MaskedPrefs{
		Prefs: ipn.Prefs{
			WantRunning: false,
		},
		WantRunningSet: true,
	})
	if err != nil {
		log_critical(fmt.Sprintf("failed bring tailscale down: %v", err))
	}
}

//export tailscale_up
func tailscale_up() {
	log_info("tailscale up")
	args := []string{"up"}
	if err := cli.Run(args); err != nil {
		log_critical(fmt.Sprintf("failed to bring tailscale up: %v", err))
	}
}

//export tailscale_status
func tailscale_status() *C.char {
	status, err := client.Status(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale status: %v", err))
		return nil
	}
	j, err := json.MarshalIndent(status, "", "  ")
	if err != nil {
		log_critical(fmt.Sprintf("failed to indent tailscale status: %v", err))
		return nil
	}

	return C.CString(string(j))
}

func main() {
}
