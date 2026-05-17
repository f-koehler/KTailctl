//go:build tools

package tools

import (
	_ "github.com/golangci/golangci-lint/v2/cmd/golangci-lint"
	_ "golang.org/x/vuln/cmd/govulncheck"
	_ "mvdan.cc/sh/v3/cmd/shfmt"
)
