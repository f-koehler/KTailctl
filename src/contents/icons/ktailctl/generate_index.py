#!/usr/bin/env python3
import argparse
from pathlib import Path
import sys


def main():
    print(sys.argv)
    parser = argparse.ArgumentParser(
        description="Generate a theme file for ktailctl",
    )
    parser.add_argument(
        "theme_file",
        type=Path,
        help="Path to the theme file to generate",
    )
    parser.add_argument(
        "sizes",
        type=str,
        help="The sizes of the icons, separated by commas",
    )
    args = parser.parse_args()

    sizes = args.sizes.split(",")

    content = "\n".join(
        [
            "[Icon Theme]",
            "Name=ktailctl",
            "Comment=Icons for KTailctl",
            "Directories=" + ",".join((f"{size}x{size}" for size in sizes)),
            "",
            "[scalable]",
            "Type=Scalable",
            "MinSize=16",
            "MaxSize=512",
        ]
    )

    for size in sizes:
        content += "\n\n" + "\n".join(
            [
                f"[{size}x{size}]",
                f"Size={size}",
                "Type=fixed",
            ]
        )

    args.theme_file.parent.mkdir(parents=True, exist_ok=True)
    args.theme_file.write_text(content)


if __name__ == "__main__":
    main()
