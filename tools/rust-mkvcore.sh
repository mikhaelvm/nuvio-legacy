#!/bin/bash
# Builds rust/mkvcore as a static library and prints the path of the .a.
# Toolchain: RUSTUP_HOME/CARGO_HOME default to the standard user install. Never
# installs anything: a missing cargo or target is an error.
set -eu
cd "$(dirname "$0")/.."
export RUSTUP_HOME="${RUSTUP_HOME:-$HOME/.rustup}"
export CARGO_HOME="${CARGO_HOME:-$HOME/.cargo}"
export PATH="$CARGO_HOME/bin:$PATH"
command -v cargo >/dev/null || { echo "rust-mkvcore: cargo not found" >&2; exit 1; }
alvo="${1:-}"
args=(--release --locked --manifest-path rust/mkvcore/Cargo.toml --target-dir "${NV_RUST_TARGET_DIR:-rust/mkvcore/target}")
[ -n "$alvo" ] && args+=(--target "$alvo")
# Keep the standalone no_std personality unless std is linked by DV7.
[ "${NUVIO_RUST_EXTERNAL_PERSONALITY:-0}" = 1 ] && args+=(--features external-personality)
cargo build -q "${args[@]}" >&2
dir="${NV_RUST_TARGET_DIR:-rust/mkvcore/target}"
if [ -n "$alvo" ]; then echo "$(pwd)/$dir/$alvo/release/libmkvcore.a"; else echo "$(pwd)/$dir/release/libmkvcore.a"; fi
