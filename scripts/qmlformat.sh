#!/bin/bash
set -euf -o pipefail

for cmd in qmlformat-qt6 qmlformat6 qmlformat; do
	if command -v "${cmd}" >/dev/null 2>&1; then
		exec "${cmd}" "$@"
	fi
done

echo "Error: no qmlformat binary found (tried qmlformat-qt6, qmlformat6, qmlformat)" >&2
exit 1
