import pycountry
import tqdm
import unicodedata
import urllib.request
import urllib.error
from pathlib import Path

for country in tqdm.tqdm(pycountry.countries):
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
