import dataclasses
from pathlib import Path


@dataclasses.dataclass
class Theme:
    name: str
    color_online: str
    color_offline: str


themes: list[Theme] = [
    Theme("breeze-dark", "#eff0f1", "#800000"),
    Theme("breeze-light", "#232629", "#800000"),
    Theme("colorful", "#27ae60", "#da4453"),
    Theme("adwaita-dark", "#26a269", "#ff7b63"),
    Theme("adwaita-light", "#c01c28", "#2ec27e"),
    Theme("catppuccin-latte", "#40a02b", "#d20f39"),
    Theme("catppuccin-frappe", "#a6d189", "#e78284"),
    Theme("catppuccin-macchiato", "#a6da95", "#ed8796"),
    Theme("catppuccin-mocha", "#a6e3a1", "#f38ba8"),
]

icon_dir = Path.cwd() / "src" / "icons"
offline_base = (icon_dir / "base" / "offline.svg").read_text()
online_base = (icon_dir / "base" / "online.svg").read_text()

qrc = """
<!--
    SPDX-License-Identifier: GPL-3.0-or-later
    SPDX-FileCopyrightText: 2024 Fabian Köhler <me@fkoehler.org>
-->
<RCC>
    <qresource prefix="/icons/">
        <file>logo.svg</file>
"""
for theme in themes:
    offline = offline_base.replace("#da4453", theme.color_offline)
    online = online_base.replace("#27ae60", theme.color_online)

    (icon_dir / ("offline-" + theme.name)).with_suffix(".svg").write_text(offline)
    (icon_dir / ("online-" + theme.name)).with_suffix(".svg").write_text(offline)
    qrc += f"        <file>offline-{theme.name}.svg</file>\n"
    qrc += f"        <file>online-{theme.name}.svg</file>\n"
qrc += """
    </qresource>
</RCC>
"""
(icon_dir / "icons.qrc").write_text(qrc)

header = (
    """
#ifndef KTAILCTL_CONFIG_THEMES_HPP
#define KTAILCTL_CONFIG_THEMES_HPP

#include <QStringList>

static const QStringList themes = {
"""
    + ", ".join(f'QStringLiteral("+{theme.name}")' for theme in themes)
    + """
};

#endif /* KTAILCTL_CONFIG_THEMES_HPP */
"""
)
(Path.cwd() / "src" / "config" / "themes.hpp").write_text(header)
