#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/.."
cc -Isrc tests/netflix_font.c -o /tmp/nuvio-netflix-font $(pkg-config --cflags --libs sdl2 SDL2_ttf)
/tmp/nuvio-netflix-font
