import argparse
from pathlib import Path
import sys
import itertools


def main():
    print(sys.argv)
    parser = argparse.ArgumentParser(
        description="Generate a theme file for ktailctl",
    )
    parser.add_argument(
        "resource_file",
        type=Path,
        help="Path to the resource file to generate",
    )
    parser.add_argument(
        "sizes",
        type=str,
        help="The sizes of the icons, separated by commas",
    )
    parser.add_argument(
        "names",
        type=str,
        help="The names of the icons, separated by commas",
    )
    args = parser.parse_args()
    print(args.sizes)

    sizes = args.sizes.split(",")
    names = args.names.split(",")

    file_entries = [
        f"      <file>icons/ktailctl/{size}x{size}/{name}.png</file>"
        for size, name in itertools.product(sizes, names)
    ]

    content = "\n".join(
        [
            "<RCC>",
            '  <qresource prefix="/">',
            "    <file>icons/ktailctl/index.theme</file>",
            "\n".join(file_entries),
            "  </qresource>",
            "</RCC>",
            "",
        ]
    )

    args.resource_file.parent.mkdir(parents=True, exist_ok=True)
    args.resource_file.write_text(content)


if __name__ == "__main__":
    main()
