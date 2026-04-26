#!/usr/bin/env -S uv run
import multiprocessing
import unicodedata
import urllib.error
import urllib.request
from pathlib import Path

import jinja2
import pycountry


def download_flag(country_code: str) -> Path | None:
    symbol1 = unicodedata.lookup("REGIONAL INDICATOR SYMBOL LETTER " + country_code[0])
    symbol2 = unicodedata.lookup("REGIONAL INDICATOR SYMBOL LETTER " + country_code[1])
    hex1 = hex(ord(symbol1)).lstrip("0x")
    hex2 = hex(ord(symbol2)).lstrip("0x")
    url = f"https://raw.githubusercontent.com/jdecked/twemoji/master/assets/svg/{hex1}-{hex2}.svg"
    print(f"Downloading {url}")
    target = Path.cwd() / "src" / "country-flags" / f"{country_code}.svg"
    target.parent.mkdir(parents=True, exist_ok=True)
    try:
        _ = urllib.request.urlretrieve(url, target)
        return target
    except urllib.error.HTTPError:
        raise RuntimeError(f"Failed to download: {url}")


def main():
    country_codes = [country.alpha_2.lower() for country in pycountry.countries]
    flag_files: list[Path] = []
    with multiprocessing.Pool() as pool:
        for result in pool.imap(download_flag, country_codes):
            if result:
                flag_files.append(result)

    flags = [file.stem for file in flag_files]

    _ = (Path.cwd() / "src" / "CountryFlags.cmake").write_text(
        jinja2.Environment(
            loader=jinja2.FileSystemLoader(Path().cwd() / "src"),
        )
        .get_template("CountryFlags.cmake.j2")
        .render(flags=flags)
    )


if __name__ == "__main__":
    main()
