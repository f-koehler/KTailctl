package main

import (
	"encoding/json"
	"os"
)

func main() {
	enc := json.NewEncoder(os.Stdout)
	enc.Encode(42)
}
