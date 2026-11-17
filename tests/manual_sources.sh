#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/.."
mkdir -p build/host-213
sources=()
for source in src/*.c src/dts/*.c; do case "$source" in src/main.c|src/streams.c) continue;; esac; sources+=("$source"); done
sources+=(tests/manual_sources.c)
flags="-O0 -g -Isrc -ffunction-sections -fdata-sections $(pkg-config --cflags sdl2 SDL2_image SDL2_ttf)"
make -f tools/native-objects.mk -j4 "SOURCES=${sources[*]}" OBJDIR=build/host-213 CC=cc "NATIVE_FLAGS=$flags" > build/host-213/compile.log 2>&1
objects=();for s in "${sources[@]}";do objects+=("build/host-213/${s%.c}.o");done
cc "${objects[@]}" -Wl,--gc-sections $(pkg-config --libs sdl2 SDL2_image SDL2_ttf) -lGLESv2 -lz -lm -ldl -pthread -o build/host-213/manual-sources
build/host-213/manual-sources "${1:-/tmp/nuvio-manual-sources.png}"
