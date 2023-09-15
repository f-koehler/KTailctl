package main

import "C"

import (
	"context"
	"encoding/json"
	"fmt"

	"tailscale.com/client/tailscale"
	"tailscale.com/cmd/tailscale/cli"
	"tailscale.com/net/interfaces"
)

var client tailscale.LocalClient

//export tailscale_down
func tailscale_down() {
	log_info("tailscale down")
	args := []string{"down"}
	if err := cli.Run(args); err != nil {
		log_critical(fmt.Sprintf("failed to bring tailscale down: %v", err))
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
func tailscale_status(status_json *string) bool {
	status, err := client.Status(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale status: %v", err))
		return false
	}
	j, err := json.MarshalIndent(status, "", "  ")
	if err != nil {
		log_critical(fmt.Sprintf("failed to indent tailscale status: %v", err))
		return false
	}
	*status_json = string(j)
	return true
}

//export tailscale_get_interface_name
func tailscale_get_interface_name(name *string) bool {
	_, iface, err := interfaces.Tailscale()
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale interfacse: %v", err))
		*name = ""
		return false
	}
	if iface == nil {
		log_critical("no tailscale interface found")
		*name = ""
		return false
	}
	*name = iface.Name
	return true
}

func main() {
}
