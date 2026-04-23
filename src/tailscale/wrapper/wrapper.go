package main

import "C"

import (
	"context"
	"encoding/json"
	"fmt"

	"tailscale.com/client/local"
	"tailscale.com/cmd/tailscale/cli"
	"tailscale.com/ipn"
)

var client local.Client

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
		log_critical(fmt.Sprintf("failed to serialize tailscale status: %v", err))
		return nil
	}

	return C.CString(string(j))
}

//export tailscale_accounts
func tailscale_accounts() *C.char {
	current, all, err := client.ProfileStatus(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get account status: %v", err))
		return nil
	}
	accounts := []ipn.LoginProfile{current}
	for _, account := range all {
		if account.ID != current.ID {
			accounts = append(accounts, account)
		}
	}
	j, err := json.MarshalIndent(struct {
		Accounts  []ipn.LoginProfile `json:"accounts"`
		CurrentID ipn.ProfileID      `json:"currentID"`
	}{accounts, current.ID}, "", " ")
	if err != nil {
		log_critical(fmt.Sprint("failed to create JSON for tailscale accounts: %v", err))
		return nil
	}
	return C.CString(string(j))
}

//export tailscale_switch_account
func tailscale_switch_account(account *string) bool {
	current, all, err := client.ProfileStatus(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get account status for siwtching: %v", err))
		return false
	}

	var profileID ipn.ProfileID
	// try to match profile ID with argument
	for _, profile := range all {
		if profile.ID == ipn.ProfileID(*account) {
			profileID = profile.ID
			break
		}
	}

	if profileID == "" {
		// try to match domain name
		for _, profile := range all {
			if profile.NetworkProfile.DomainName == *account {
				profileID = profile.ID
				break
			}
		}
	}

	if profileID == "" {
		// try to match name
		for _, profile := range all {
			if profile.Name == *account {
				profileID = profile.ID
			}
		}
	}

	if profileID == "" {
		log_critical(fmt.Sprintf("failed to find profile for account %v", *account))
		return false
	}

	if profileID == current.ID {
		log_warning(fmt.Sprintf("account %v already active", *account))
		return true
	}

	if err := client.SwitchProfile(context.Background(), profileID); err != nil {
		log_critical(fmt.Sprintf("failed to switch profile: %v", err))
		return false
	}

	log_info("switched account to " + *account)

	return true
}

func main() {
}
