"""Regenerate the translation table, then rebuild the DLL."""
from pathlib import Path
import json

root = Path(__file__).resolve().parent.parent
translations = json.loads((root / 'localization.json').read_text(encoding='utf-8'))
assert next(iter(translations)) == 'en'
rows = []
for language, (title, decimal) in translations.items():
    assert language.isascii() and language.isalpha() and language.islower()
    assert title and '\n' not in title and '\r' not in title
    assert decimal in ('.', ',')
    literal = '"' + ''.join('\\x%02x' % b for b in title.encode('utf-8')) + '"'
    rows.append('    {"%s", %s, \'%s\'},' % (language, literal, decimal))
header = root / 'src/RaceLocalization.h'
source = header.read_text(encoding='ascii')
start = source.index('static const RaceLocale raceLocales[] = {')
body = source.index('\n', start) + 1
end = source.index('};', body)
header.write_text(source[:body] + '\n'.join(rows) + '\n' + source[end:], encoding='ascii')
print('Generated %d translations; rebuild the DLL to apply them.' % len(rows))
