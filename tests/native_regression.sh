#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/.."
test_source="$1"; shift
sources=()
for source in src/*.c src/dts/*.c; do
  [ "$source" = src/main.c ] && continue
  skip=0; for excluded in "$@"; do [ "$source" = "$excluded" ] && skip=1; done
  [ "$skip" = 1 ] || sources+=("$source")
done
sources+=("$test_source")
mkdir -p build/host-213
flags="-O0 -g -Isrc -ffunction-sections -fdata-sections $(pkg-config --cflags sdl2 SDL2_image SDL2_ttf)"
make -f tools/native-objects.mk -j4 "SOURCES=${sources[*]}" OBJDIR=build/host-213 CC=cc "NATIVE_FLAGS=$flags" > build/host-213/regression.log 2>&1
objects=(); for s in "${sources[@]}"; do objects+=("build/host-213/${s%.c}.o"); done
out="build/host-213/$(basename "$test_source" .c)"
cc "${objects[@]}" -Wl,--gc-sections $(pkg-config --libs sdl2 SDL2_image SDL2_ttf) -lGLESv2 -lEGL -lz -lm -ldl -pthread -o "$out"
"$out"
