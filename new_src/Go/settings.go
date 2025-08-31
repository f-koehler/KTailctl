package main

import (
	"C"
	"context"
	"encoding/json"
	"fmt"

	"tailscale.com/client/local"
)

var client local.Client

//export tailscale_get_settings
func tailscale_get_settings() *C.char {
	settings, err := client.GetPrefs(context.Background())
	if err != nil {
		log_critical(fmt.Sprintf("failed to get current tailscale preferences: %v", err))
		return nil
	}
	j, err := json.Marshal(settings)
	if err != nil {
		log_critical(fmt.Sprintf("failed to convert preferences to JSON: %v", err))
		return nil
	}
	return C.CString(string(j))
}
