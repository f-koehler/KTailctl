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

func apply_prefs(newPrefs *ipn.MaskedPrefs) bool {
	ctx := context.Background()
	curPrefs, err := client.GetPrefs(ctx)
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale preferences: %v", err))
		return false
	}
	curPrefs.ApplyEdits(newPrefs)
	if err = client.CheckPrefs(ctx, curPrefs); err != nil {
		log_critical(fmt.Sprintf("failed to check tailscale preferences: %v", err))
		return false
	}
	_, err = client.EditPrefs(ctx, newPrefs)
	if err != nil {
		log_critical(fmt.Sprintf("failed to set tailscale preferences: %v", err))
		return false
	}
	return true
}

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

//export tailscale_get_exit_node_allow_lan_access
func tailscale_get_exit_node_allow_lan_access(allow_lan_access *bool) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale preferences: %v", err))
		return false
	}
	*allow_lan_access = curPrefs.ExitNodeAllowLANAccess
	return true
}

//export tailscale_set_exit_node_allow_lan_access
func tailscale_set_exit_node_allow_lan_access(allow_lan_access *bool) bool {
	args := []string{"set", "--exit-node-allow-lan-access=" + strconv.FormatBool(*allow_lan_access)}
	err := cli.Run(args)
	if err != nil {
		log_critical(fmt.Sprintf("failed to set allow LAN access: %v", err))
		return false
	}
	log_info(fmt.Sprintf("set allow LAN access to %v", *allow_lan_access))
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
	if *exit_node != "" {
		err = prefs.Prefs.SetExitNodeIP(*exit_node, status)
	} else {
		prefs.Prefs.ClearExitNode()
	}
	prefs.ExitNodeIPSet = true
	prefs.ExitNodeIDSet = true
	if err != nil {
		log_critical(fmt.Sprintf("failed to set exit node in prefs object: %v", err))
		return false
	}

	log_info(fmt.Sprintf("set exit node to %v", *exit_node))
	return apply_prefs(prefs)
}

//export tailscale_suggest_exit_node
func tailscale_suggest_exit_node() *C.char {
	res, err := client.SuggestExitNode(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to suggest exit node: %v", err))
		return nil
	}
	return C.CString(string(res.ID))
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

	log_info(fmt.Sprintf("set accept dns to %v", *accept_dns))
	return apply_prefs(&ipn.MaskedPrefs{
		Prefs: ipn.Prefs{
			CorpDNS: *accept_dns,
		},
		CorpDNSSet: true,
	})
}

//export tailscale_get_hostname
func tailscale_get_hostname() *C.char {
	status, err := client.Status(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale status to determine hostname: %v", err))
		return nil
	}
	return C.CString(status.Self.HostName)
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

	log_info(fmt.Sprintf("set hostname to %v", *hostname))
	return apply_prefs(&ipn.MaskedPrefs{
		Prefs: ipn.Prefs{
			Hostname: *hostname,
		},
		HostnameSet: true,
	})
}

//export tailscale_get_operator_user
func tailscale_get_operator_user() *C.char {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale preferences: %v", err))
		return nil
	}
	return C.CString(curPrefs.OperatorUser)
}

//export tailscale_set_operator_user
func tailscale_set_operator_user(user *string) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale preferences: %v", err))
		return false
	}

	if curPrefs.OperatorUser == *user {
		log_warning(fmt.Sprintf("operator user already set to %v", *user))
		return true
	}

	log_info(fmt.Sprintf("set operator user to %v", *user))
	return apply_prefs(&ipn.MaskedPrefs{
		Prefs: ipn.Prefs{
			OperatorUser: *user,
		},
		OperatorUserSet: true,
	})
}

//export tailscale_is_operator
func tailscale_is_operator() bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale preferences: %v", err))
		return false
	}

	user, err := user.Current()
	if err != nil {
		log_critical(fmt.Sprintf("failed to get current user: %v", err))
		return false
	}

	return user.Username == curPrefs.OperatorUser
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

	log_info(fmt.Sprintf("set shields up to %v", *shields_up))
	return apply_prefs(&ipn.MaskedPrefs{
		Prefs: ipn.Prefs{
			ShieldsUp: *shields_up,
		},
		ShieldsUpSet: true,
	})
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

	log_info(fmt.Sprintf("set ssh to %v", *ssh))
	return apply_prefs(&ipn.MaskedPrefs{
		Prefs: ipn.Prefs{
			RunSSH: *ssh,
		},
		RunSSHSet: true,
	})
}

//export tailscale_get_webclient
func tailscale_get_webclient(webclient *bool) bool {
	curPrefs, err := client.GetPrefs(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get tailscale preferences: %v", err))
		return false
	}
	*webclient = curPrefs.RunWebClient
	return true
}

//export tailscale_set_webclient
func tailscale_set_webclient(webclient *bool) bool {
	var cur bool
	tailscale_get_webclient(&cur)
	if cur == *webclient {
		log_warning(fmt.Sprintf("webclient already set to %v", *webclient))
		return true
	}

	log_info(fmt.Sprintf("set webclient to %v", *webclient))
	return apply_prefs(&ipn.MaskedPrefs{
		Prefs: ipn.Prefs{
			RunWebClient: *webclient,
		},
		RunWebClientSet: true,
	})
}
