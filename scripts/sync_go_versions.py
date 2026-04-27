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
from collections.abc import Iterator
from pathlib import Path
from typing import TypedDict

from ruamel.yaml import YAML

PATH_REPO_ROOT = Path(__file__).resolve().parent.parent
PATH_WRAPPER_GO_MOD = PATH_REPO_ROOT / "src" / "tailscale" / "wrapper" / "go.mod"
PATH_TOOLS_GO_MOD = PATH_REPO_ROOT / "tools" / "go.mod"
PATH_PRE_COMMIT_CONFIG = PATH_REPO_ROOT / ".pre-commit-config.yaml"
REQUIRE_RE = re.compile(r"^\s+([\w./@\-]+)\s+(v\S+)", re.MULTILINE)


class Hook(TypedDict, total=False):
    id: str
    name: str
    description: str
    language: str
    language_version: str
    entry: str
    args: list[str]
    types: list[str]
    files: str
    pass_filenames: bool
    additional_dependencies: list[str]


class Repo(TypedDict, total=False):
    repo: str
    rev: str
    hooks: list[Hook]


class CiConfig(TypedDict, total=False):
    skip: list[str]


class PreCommitConfig(TypedDict, total=False):
    ci: CiConfig
    repos: list[Repo]


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


def golang_hooks(pre_commit_config: PreCommitConfig) -> Iterator[Hook]:
    """Yield every hook dict whose language is 'golang'."""
    for repo in pre_commit_config.get("repos", []):
        for hook in repo.get("hooks", []):
            if hook.get("language") == "golang":
                yield hook


def sync_language_version(pre_commit_config: PreCommitConfig, go_version: str) -> bool:
    """Update language_version on all golang hooks. Returns True if anything changed."""
    changed = False
    for hook in golang_hooks(pre_commit_config):
        if hook.get("language_version") != go_version:
            hook["language_version"] = go_version
            changed = True
    return changed


def sync_additional_deps(
    pre_commit_config: PreCommitConfig, modules: dict[str, str]
) -> bool:
    """Update additional_dependencies versions on all golang hooks. Returns True if anything changed."""
    changed = False
    for hook in golang_hooks(pre_commit_config):
        additional_deps: list[str] = hook.get("additional_dependencies", [])
        for i, dep in enumerate(additional_deps):
            dep_path, old_ver = dep.split("@", 1)
            new_ver = find_module_version(dep_path, modules)
            if new_ver is None or new_ver == old_ver:
                continue
            additional_deps[i] = f"{dep_path}@{new_ver}"
            changed = True
    return changed


def main() -> int:
    go_version = parse_go_directive(PATH_WRAPPER_GO_MOD)
    tool_modules = parse_requires(PATH_TOOLS_GO_MOD)

    any_changed = False

    # Sync go directive in tools/go.mod
    tools_text = PATH_TOOLS_GO_MOD.read_text()
    new_tools_text, changed = sync_go_directive(tools_text, go_version)
    if changed:
        _ = PATH_TOOLS_GO_MOD.write_text(new_tools_text)
        print(f"tools/go.mod: updated go directive to {go_version}")
        any_changed = True

    # Sync .pre-commit-config.yaml
    yaml = YAML()  # round-trip: preserves comments and formatting
    yaml.preserve_quotes = True
    # ruamel.yaml's load() is untyped; cast at the boundary and suppress the warnings.
    pre_commit_config: PreCommitConfig = yaml.load(PATH_PRE_COMMIT_CONFIG)  # pyright: ignore[reportUnknownMemberType, reportAny]

    language_version_changed = sync_language_version(pre_commit_config, go_version)
    dependencies_changed = sync_additional_deps(pre_commit_config, tool_modules)

    if language_version_changed:
        print(f".pre-commit-config.yaml: updated language_version to {go_version}")
        any_changed = True
    if dependencies_changed:
        print(".pre-commit-config.yaml: updated additional_dependencies versions")
        any_changed = True

    if language_version_changed or dependencies_changed:
        with PATH_PRE_COMMIT_CONFIG.open("w") as f:
            yaml.dump(pre_commit_config, f)  # pyright: ignore[reportUnknownMemberType]

    if any_changed:
        print("Files were modified - re-stage and re-commit.")
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
