package main

import "C"

import (
	"context"
	"encoding/json"

	"tailscale.com/client/tailscale"
	"tailscale.com/cmd/tailscale/cli"
	"tailscale.com/net/interfaces"
)

var client tailscale.LocalClient

//export tailscale_down
func tailscale_down() {
	args := []string{"down"}
	if err := cli.Run(args); err != nil {
		log_critical_error(err)
	}
}

//export tailscale_up
func tailscale_up() {
	args := []string{"up"}
	if err := cli.Run(args); err != nil {
		log_critical_error(err)
	}
}

//export tailscale_status
func tailscale_status(status_json *string) bool {
	status, err := client.Status(context.Background())
	if err != nil {
		log_critical_error(err)
		return false
	}
	j, err := json.MarshalIndent(status, "", "  ")
	if err != nil {
		log_critical_error(err)
		return false
	}
	*status_json = string(j)
	return true
}

//export tailscale_get_interface_name
func tailscale_get_interface_name(name *string) bool {
	_, iface, err := interfaces.Tailscale()
	if err != nil {
		log_critical_error(err)
		*name = ""
		return false
	}
	if iface == nil {
		log_critical_error(err)
		*name = ""
		return false
	}
	*name = iface.Name
	return true
}

func main() {
}
