//go:build tools

package tools

import (
	_ "github.com/go-critic/go-critic/cmd/gocritic"
	_ "github.com/securego/gosec/v2/cmd/gosec"
	_ "golang.org/x/tools/cmd/goimports"
	_ "honnef.co/go/tools/cmd/staticcheck"
	_ "mvdan.cc/sh/v3/cmd/shfmt"
)
