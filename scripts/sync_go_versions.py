#!/usr/bin/env -S uv run
"""
Sync Go version and tool versions across configuration files.

Sources of truth:
  - Go version:       src/tailscale/wrapper/go.mod  (go directive)
  - Go tool versions: tools/go.mod                  (require directives)

Targets:
  - tools/go.mod:            go directive
  - .pre-commit-config.yaml: language_version fields (all local golang hooks)
  - .pre-commit-config.yaml: additional_dependencies versions (all local golang hooks)

Exit 0 if nothing changed, exit 1 if any file was modified (pre-commit convention).
"""

import re
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
WRAPPER_GO_MOD = REPO_ROOT / "src" / "tailscale" / "wrapper" / "go.mod"
TOOLS_GO_MOD = REPO_ROOT / "tools" / "go.mod"
PRE_COMMIT_CONFIG = REPO_ROOT / ".pre-commit-config.yaml"
REQUIRE_RE = re.compile(r"^\s+([\w./@\-]+)\s+(v\S+)", re.MULTILINE)


def parse_go_directive(path: Path) -> str:
    for line in path.read_text().splitlines():
        line = line.strip()
        if line.startswith("go ") and len(line.split()) == 2:
            return line.split()[1]
    raise ValueError(f"No go directive found in {path}")


def parse_requires(path: Path) -> dict[str, str]:
    """Return {module_path: version} for all require entries (direct and indirect)."""
    return {m.group(1): m.group(2) for m in REQUIRE_RE.finditer(path.read_text())}


def find_module_version(dep_path: str, modules: dict[str, str]) -> str | None:
    """Find the version for the longest module path that is a prefix of dep_path."""
    best_mod: str | None = None
    for mod in modules:
        if dep_path == mod or dep_path.startswith(mod + "/"):
            if best_mod is None or len(mod) > len(best_mod):
                best_mod = mod
    return modules[best_mod] if best_mod is not None else None


def sync_go_directive(text: str, go_version: str) -> tuple[str, bool]:
    new = re.sub(r"^go\s+[\d.]+", f"go {go_version}", text, flags=re.MULTILINE)
    return new, new != text


def sync_language_version(text: str, go_version: str) -> tuple[str, bool]:
    new = re.sub(
        r"(language_version:\s*)[\d.]+",
        lambda m: m.group(1) + go_version,
        text,
    )
    return new, new != text


def sync_additional_deps(text: str, modules: dict[str, str]) -> tuple[str, bool]:
    changed = False

    def replace(match: re.Match[str]) -> str:
        nonlocal changed
        dep_path, old_ver = match.group(1), match.group(2)
        new_ver = find_module_version(dep_path, modules)
        if new_ver is None or new_ver == old_ver:
            return match.group(0)
        changed = True
        return f'"{dep_path}@{new_ver}"'

    new = re.sub(r'"([\w./@\-]+)@(v[^\s"]+)"', replace, text)
    return new, changed


def main() -> int:
    go_version = parse_go_directive(WRAPPER_GO_MOD)
    tool_modules = parse_requires(TOOLS_GO_MOD)

    any_changed = False

    # Sync go directive in tools/go.mod
    tools_text = TOOLS_GO_MOD.read_text()
    new_tools_text, changed = sync_go_directive(tools_text, go_version)
    if changed:
        _ = TOOLS_GO_MOD.write_text(new_tools_text)
        print(f"tools/go.mod: updated go directive to {go_version}")
        any_changed = True

    # Sync .pre-commit-config.yaml
    pc_text = PRE_COMMIT_CONFIG.read_text()
    pc_text, lv_changed = sync_language_version(pc_text, go_version)
    pc_text, dep_changed = sync_additional_deps(pc_text, tool_modules)

    if lv_changed:
        print(f".pre-commit-config.yaml: updated language_version to {go_version}")
        any_changed = True
    if dep_changed:
        print(".pre-commit-config.yaml: updated additional_dependencies versions")
        any_changed = True

    if lv_changed or dep_changed:
        _ = PRE_COMMIT_CONFIG.write_text(pc_text)

    if any_changed:
        print("Files were modified — re-stage and re-commit.")
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
