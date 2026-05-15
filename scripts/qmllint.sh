#!/bin/bash
set -euf -o pipefail

for cmd in qmllint-qt6 qmllint6 qmllint; do
	if command -v "${cmd}" >/dev/null 2>&1; then
		exec "${cmd}" "$@"
	fi
done

echo "Error: no qmllint binary found (tried qmllint-qt6, qmllint6, qmllint)" >&2
exit 1
