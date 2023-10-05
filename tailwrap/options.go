package main

import "C"
import (
	"context"
	"fmt"
	"os/user"
	"strconv"

	"tailscale.com/cmd/tailscale/cli"
	"tailscale.com/ipn"
)

//export tailscale_get_accept_routes
func tailscale_get_accept_routes(accept_routes *bool) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale preferences: %v", err))
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
		log_critical(fmt.Sprintf("failed to set accept routes: %v", err))
		return false
	}
	log_info(fmt.Sprintf("set accept routes to %v", *accept_routes))
	return true
}

//export tailscale_get_advertise_exit_node
func tailscale_get_advertise_exit_node(exit_node *bool) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale preferences: %v", err))
		return false
	}
	*exit_node = curPrefs.AdvertisesExitNode()
	return true
}

//export tailscale_set_advertise_exit_node
func tailscale_set_advertise_exit_node(exit_node *bool) bool {
	args := []string{"set", "--advertise-exit-node=" + strconv.FormatBool(*exit_node)}
	if err := cli.Run(args); err != nil {
		log_critical(fmt.Sprintf("failed to set advertise exit node: %v", err))
		return false
	}
	log_info(fmt.Sprintf("set advertise exit node to %v", *exit_node))
	return true
}

//export tailscale_set_exit_node
func tailscale_set_exit_node(exit_node *string) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale preferences: %v", err))
		return false
	}

	if curPrefs.ExitNodeIP.String() == *exit_node {
		log_warning(fmt.Sprintf("exit node already set to %v", *exit_node))
		return false
	}

	status, err := client.Status(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale status: %v", err))
		return false
	}

	prefs := &ipn.MaskedPrefs{}
	err = prefs.Prefs.SetExitNodeIP(*exit_node, status)
	if err != nil {
		log_critical(fmt.Sprintf("failed to set exit node in prefs object: %v", err))
		return false
	}

	_, err = client.EditPrefs(context.Background(), prefs)
	if err != nil {
		log_critical(fmt.Sprintf("failed to set exit node: %v", err))
		return false
	}

	log_info(fmt.Sprintf("set exit node to %v", *exit_node))
	return true
}

//export tailscale_get_accept_dns
func tailscale_get_accept_dns(accept_dns *bool) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale preferences: %v", err))
		return false
	}
	*accept_dns = curPrefs.CorpDNS
	return true
}

//export tailscale_set_accept_dns
func tailscale_set_accept_dns(accept_dns *bool) bool {
	var cur bool
	tailscale_get_accept_dns(&cur)
	if cur == *accept_dns {
		log_warning(fmt.Sprintf("accept dns already set to %v", *accept_dns))
		return true
	}

	_, err := client.EditPrefs(context.Background(), &ipn.MaskedPrefs{
		Prefs: ipn.Prefs{
			CorpDNS: *accept_dns,
		},
		CorpDNSSet: true,
	})
	if err != nil {
		log_critical(fmt.Sprintf("failed to set accept dns: %v", err))
		return false
	}

	log_info(fmt.Sprintf("set accept dns to %v", *accept_dns))
	return true
}

//export tailscale_get_hostname
func tailscale_get_hostname(hostname *string) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale preferences: %v", err))
		return false
	}
	*hostname = curPrefs.Hostname
	return true
}

//export tailscale_set_hostname
func tailscale_set_hostname(hostname *string) bool {
	ctx := context.Background()
	status, err := client.Status(ctx)
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale status: %v", err))
		return false
	}

	if status.Self.HostName == *hostname {
		log_warning(fmt.Sprintf("hostname already set to %v", *hostname))
		return true
	}

	_, err = client.EditPrefs(ctx, &ipn.MaskedPrefs{
		Prefs: ipn.Prefs{
			Hostname: *hostname,
		},
		HostnameSet: true,
	})

	if err != nil {
		log_critical(fmt.Sprintf("failed to set hostname: %v", err))
		return false
	}

	log_info(fmt.Sprintf("set hostname to %v", *hostname))
	return true
}

//export tailscale_get_operator_user
func tailscale_get_operator_user(user *string) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale preferences: %v", err))
		return false
	}
	*user = curPrefs.OperatorUser
	return true
}

//export tailscale_set_operator_user
func tailscale_set_operator_user(user *string) bool {
	var cur string
	tailscale_get_operator_user(&cur)
	if cur == *user {
		log_warning(fmt.Sprintf("operator user already set to %v", *user))
		return true
	}

	_, err := client.EditPrefs(context.Background(), &ipn.MaskedPrefs{
		Prefs: ipn.Prefs{
			OperatorUser: *user,
		},
		OperatorUserSet: true,
	})
	if err != nil {
		log_critical(fmt.Sprintf("failed to set operator user: %v", err))
		return false
	}

	log_info(fmt.Sprintf("set operator user to %v", *user))
	return true
}

//export tailscale_is_operator
func tailscale_is_operator() bool {
	var operator string
	if !tailscale_get_operator_user(&operator) {
		log_critical("failed to get operator user")
		return false
	}

	user, err := user.Current()
	if err != nil {
		log_critical(fmt.Sprintf("failed to get current user: %v", err))
		return false
	}

	return user.Username == operator
}

//export tailscale_get_shields_up
func tailscale_get_shields_up(shields_up *bool) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale preferences: %v", err))
		return false
	}
	*shields_up = curPrefs.ShieldsUp
	return true
}

//export tailscale_set_shields_up
func tailscale_set_shields_up(shields_up *bool) bool {
	var cur bool
	tailscale_get_shields_up(&cur)
	if cur == *shields_up {
		log_warning(fmt.Sprintf("shields-up already set to %v", *shields_up))
		return true
	}

	_, err := client.EditPrefs(context.Background(), &ipn.MaskedPrefs{
		Prefs: ipn.Prefs{
			ShieldsUp: *shields_up,
		},
		ShieldsUpSet: true,
	})
	if err != nil {
		log_critical(fmt.Sprintf("failed to set shields up: %v", err))
		return false
	}

	log_info(fmt.Sprintf("set shields up to %v", *shields_up))
	return true
}

//export tailscale_get_ssh
func tailscale_get_ssh(ssh *bool) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale preferences: %v", err))
		return false
	}
	*ssh = curPrefs.RunSSH
	return true
}

//export tailscale_set_ssh
func tailscale_set_ssh(ssh *bool) bool {
	var cur bool
	tailscale_get_ssh(&cur)
	if cur == *ssh {
		log_warning(fmt.Sprintf("ssh already set to %v", *ssh))
		return true
	}

	_, err := client.EditPrefs(context.Background(), &ipn.MaskedPrefs{
		Prefs: ipn.Prefs{
			RunSSH: *ssh,
		},
		RunSSHSet: true,
	})
	if err != nil {
		log_critical(fmt.Sprintf("failed to set ssh: %v", err))
		return false
	}

	log_info(fmt.Sprintf("set ssh to %v", *ssh))
	return true
}
