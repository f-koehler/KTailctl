package main

import "C"

import (
	"context"
	"encoding/json"
	"fmt"
	"os"

	"tailscale.com/client/tailscale"
	"tailscale.com/cmd/tailscale/cli"
)

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
func tailscale_receive_files(strategy string, directory string) {
	args := []string{"file", "get", "-loop=true", "-verbose=true", "-wait=true", "-conflict", strategy, directory}
	if err := cli.Run(args); err != nil {
		fmt.Fprintln(os.Stderr, err)
	}
}

//export tailscale_status
func tailscale_status(status_json *string) bool {
	var client tailscale.LocalClient
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

//export tailscale_get_hostname
func tailscale_get_hostname(hostname *string) bool {
	var client tailscale.LocalClient
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	*hostname = curPrefs.Hostname
	return true
}

//export tailscale_get_operator_user
func tailscale_get_operator_user(user *string) bool {
	var client tailscale.LocalClient
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	*user = curPrefs.OperatorUser
	return true
}

func main() {
}
