#!/usr/bin/env -S uv run
import pycountry
import unicodedata
import urllib.request
import urllib.error
from pathlib import Path
import logging
from rich.progress import track
from rich.logging import RichHandler    

def main():
    logging.info("Downloading %d country flags", len(pycountry.countries))
    for country in track(pycountry.countries):
        code = country.alpha_2.lower()
        symbol1 = unicodedata.lookup("REGIONAL INDICATOR SYMBOL LETTER " + code[0])
        symbol2 = unicodedata.lookup("REGIONAL INDICATOR SYMBOL LETTER " + code[1])
        hex1 = hex(ord(symbol1)).lstrip("0x")
        hex2 = hex(ord(symbol2)).lstrip("0x")
        try:
            urllib.request.urlretrieve(
                f"https://raw.githubusercontent.com/twitter/twemoji/master/assets/svg/{hex1}-{hex2}.svg",
                Path.cwd().resolve() / f"{code}.svg",
            )
        except urllib.error.HTTPError:
            continue

if __name__ == "__main__":
    logging.basicConfig(
        level="NOTSET", format="%(message)s", datefmt="[%X]", handlers=[RichHandler()]
    )
    main()