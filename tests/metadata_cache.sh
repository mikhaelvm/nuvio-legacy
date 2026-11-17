#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/.."
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
python3 - "$work" <<'PY'
from pathlib import Path
import sys
s=Path('src/trakt.c').read_text()
a=s.index('#define TK_FICHA_MAX'); b=s.index('// CACHE NEGATIVO',a)
Path(sys.argv[1]+'/cache.inc').write_text(s[a:b])
PY
cc -O1 -g -Wall -Wextra -I"$work" tests/metadata_cache.c -pthread -o "$work/test"
"$work/test"
