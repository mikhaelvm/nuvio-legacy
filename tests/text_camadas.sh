#!/bin/bash
# Two zoomed text layers in one frame keep their lines (no per-frame re-raster).
#   bash tests/text_camadas.sh
set -eu
cd "$(dirname "$0")/.."
sources=()
for source in src/*.c src/dts/*.c; do [ "$source" != src/main.c ] && sources+=("$source"); done
bin="${TMPDIR:-/tmp}/nuvio-text-camadas"
if [ "$(uname -s)" = Darwin ]; then
  flags=(-I/opt/homebrew/include -I/opt/homebrew/include/SDL2 -L/opt/homebrew/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lz -framework OpenGL)
else
  flags=($(pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf) -lGLESv2 -lz -lm -ldl -pthread)
fi
cc "${sources[@]}" tests/text_camadas.c -Isrc -o "$bin" -O1 -g "${flags[@]}"
NUVIO_DADOS=$(mktemp -d) "$bin"
