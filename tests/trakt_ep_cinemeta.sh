#!/bin/bash
# #356: episodio "a seguir" ausente na ficha do Nuvio mas presente no Cinemeta.
#   bash tests/trakt_ep_cinemeta.sh
set -euo pipefail
strip_flag=-Wl,-dead_strip
[ "$(uname -s)" = Darwin ] || strip_flag="-Wl,--gc-sections -ffunction-sections -fdata-sections"
cd "$(dirname "$0")/.."
dir=$(mktemp -d "${TMPDIR:-/tmp}/nuvio-trakt-ep.XXXXXX")
trap 'rm -rf "$dir"' EXIT
cc src/trakt.c src/metaep.c tests/stub_fichameta.c tests/stub_rede_legacy.c src/js.c src/metaprov.c tests/trakt_ep_cinemeta.c \
  -Isrc -I/opt/homebrew/include -I/opt/homebrew/include/SDL2 \
  -o "$dir/teste" -O1 -g -Wall -Wextra $strip_flag -lpthread
"$dir/teste" | tail -3
