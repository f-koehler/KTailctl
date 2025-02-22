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
import jinja2

def download_flag(country_code: str) -> Path | None:
    symbol1 = unicodedata.lookup("REGIONAL INDICATOR SYMBOL LETTER " + country_code[0])
    symbol2 = unicodedata.lookup("REGIONAL INDICATOR SYMBOL LETTER " + country_code[1])
    hex1 = hex(ord(symbol1)).lstrip("0x")
    hex2 = hex(ord(symbol2)).lstrip("0x")
    url = f"https://raw.githubusercontent.com/jdecked/twemoji/master/assets/svg/{hex1}-{hex2}.svg"
    target = Path(__file__).resolve().parent / "country-flags" / f"{country_code}.svg"
    target.parent.mkdir(parents=True, exist_ok=True)
    try:
        urllib.request.urlretrieve(
            url,
            target
        )
        return target
    except urllib.error.HTTPError:
        logging.error("Failed to download: %s", url)

def main():
    country_codes = [country.alpha_2.lower() for country in pycountry.countries]    
    flag_files: list[str] = []
    with Progress() as progress:
        task_id = progress.add_task("Downloading country flags", total=len(country_codes))
        with multiprocessing.Pool() as pool:
            for result in pool.imap(download_flag, country_codes):
                progress.advance(task_id)
                if result:
                    flag_files.append(result)
    
    flags = [file.stem for file in flag_files]
    
    (Path(__file__).resolve().parent / "CountryFlags.cmake").write_text(
        jinja2.Environment(
            loader=jinja2.FileSystemLoader(Path(__file__).resolve().parent),
        ).get_template("CountryFlags.cmake.j2").render(flags=flags)
    )

if __name__ == "__main__":
    logging.basicConfig(
        level="NOTSET", format="%(message)s", datefmt="[%X]", handlers=[RichHandler()]
    )
    main()