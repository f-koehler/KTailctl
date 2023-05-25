package main

import "C"

import (
	"context"
	"encoding/json"
	"fmt"
	"os"
	"strconv"

	"tailscale.com/client/tailscale"
	"tailscale.com/cmd/tailscale/cli"
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
func tailscale_receive_files(strategy string, directory string) {
	args := []string{"file", "get", "-loop=true", "-verbose=true", "-wait=true", "-conflict", strategy, directory}
	if err := cli.Run(args); err != nil {
		fmt.Fprintln(os.Stderr, err)
	}
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
	args := []string{"set", "--accept-routes", strconv.FormatBool(*accept_routes)}
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
	args := []string{"set", "--accept-dns", strconv.FormatBool(*accept_dns)}
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
	fmt.Println("Setting hostname to", *hostname)
	args := []string{"set", "--hostname", *hostname}
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
	args := []string{"set", "--operator", *user}
	if err := cli.Run(args); err != nil {
		fmt.Fprintln(os.Stderr, err)
		return false
	}
	return true
}

func main() {
}
