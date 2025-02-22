#!/usr/bin/env -S uv run
import pycountry
import unicodedata
import urllib.request
import urllib.error
from pathlib import Path
import logging
from rich.progress import Progress
from rich.logging import RichHandler    
import multiprocessing

def download_flag(country_code: str):
    symbol1 = unicodedata.lookup("REGIONAL INDICATOR SYMBOL LETTER " + country_code[0])
    symbol2 = unicodedata.lookup("REGIONAL INDICATOR SYMBOL LETTER " + country_code[1])
    hex1 = hex(ord(symbol1)).lstrip("0x")
    hex2 = hex(ord(symbol2)).lstrip("0x")
    url = f"https://raw.githubusercontent.com/twitter/twemoji/master/assets/svg/{hex1}-{hex2}.svg"
    try:
        urllib.request.urlretrieve(
            url,
            Path.cwd().resolve() / f"{country_code}.svg",
        )
    except urllib.error.HTTPError:
        logging.error("Failed to download: %s", url)

def main():
    country_codes = [country.alpha_2.lower() for country in pycountry.countries]    
    with Progress() as progress:
        task_id = progress.add_task("Downloading country flags", total=len(country_codes))
        with multiprocessing.Pool() as pool:
            for _ in pool.imap(download_flag, country_codes):
                progress.advance(task_id)

if __name__ == "__main__":
    logging.basicConfig(
        level="NOTSET", format="%(message)s", datefmt="[%X]", handlers=[RichHandler()]
    )
    main()