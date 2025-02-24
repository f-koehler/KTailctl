#!/usr/bin/env -S uv run
import dataclasses
from pathlib import Path


@dataclasses.dataclass
class Theme:
    name: str
    color_online: str
    color_offline: str


themes: list[Theme] = [
    Theme("breeze-light", "#232629", "#da4453"),
    Theme("breeze-dark", "#eff0f1", "#eff0f1"),
    Theme("colorful", "#27ae60", "#da4453"),
    Theme("adwaita-dark", "#26a269", "#ff7b63"),
    Theme("adwaita-light", "#2ec27e", "#c01c28"),
    Theme("catppuccin-latte", "#8839ef", "#8839ef"),
    Theme("catppuccin-frappe", "#ca9ee6", "#ca9ee6"),
    Theme("catppuccin-macchiato", "#c6a0f6", "#c6a0f6"),
    Theme("catppuccin-mocha", "#cba6f7", "#cba6f7"),
]

if __name__ == "__main__":
    icon_dir = Path.cwd() / "src" / "icons"
    offline_base = (icon_dir / "base" / "offline.svg").read_text()
    online_base = (icon_dir / "base" / "online.svg").read_text()
    exit_node_base = (icon_dir / "base" / "exit-node.svg").read_text()

    for theme in themes:
        offline = offline_base.replace("#232629", theme.color_offline)
        online = online_base.replace("#232629", theme.color_online)
        exit_node = exit_node_base.replace("#232629", theme.color_online)

        (icon_dir / ("offline-" + theme.name)).with_suffix(".svg").write_text(offline)
        (icon_dir / ("online-" + theme.name)).with_suffix(".svg").write_text(online)
        (icon_dir / ("exit-node-" + theme.name)).with_suffix(".svg").write_text(exit_node)

    header = (
        """
    #ifndef KTAILCTL_GENERATE_ICONS_PY
    #define KTAILCTL_GENERATE_ICONS_PY

    #include <QStringList>

    static const QStringList TrayIconThemes = {
    """
        + ", ".join(f'QStringLiteral("{theme.name}")' for theme in themes)
        + """
    };

    #endif /* KTAILCTL_GENERATE_ICONS_PY */
    """
    )
    (Path.cwd() / "src" / "themes.hpp").write_text(header)
