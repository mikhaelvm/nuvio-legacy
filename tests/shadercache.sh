#!/bin/bash
set -euo pipefail
cd "$(dirname "$0")/.."
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT
flags=()
if [ "${SANITIZE:-0}" = 1 ]; then flags=(-fsanitize=address,undefined -fno-omit-frame-pointer); fi
cc -O1 -g -Wall -Wextra "${flags[@]}" -Isrc src/shadercache.c tests/shadercache.c $(pkg-config --cflags sdl2) -o "$work/test"
"$work/test"
