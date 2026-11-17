#!/bin/sh
export TMPDIR="${TMPDIR:-/tmp}"
TMPDIR="${TMPDIR%/}"
# Minimal static LGPL FFmpeg: DTS decode, stereo AAC and AC3 5.1 encode (no GPL/nonfree). No FFmpeg
# network/TLS protocols or software video decoders enter the TV executable.
#
# Cross build with the LG SDK; NUVIO_DTS_HOST=1 builds host test fixtures.
set -eu
VERSION=7.1.5
SHA256=de668509caf9e35e3cd162473441fdb29538c6d96ed080292b3cf9e6fc5d558f
PREFIX=${NUVIO_DTS_ROOT:-/opt/nuvio-dts}
BUILD=${NUVIO_DTS_BUILD:-${TMPDIR:-/tmp}/nuvio-dts-build}
DECODERS=dca,dvdsub,pgssub,ass,ssa,subrip,movtext,webvtt
MUXERS=
PARSERS=dca,h264,hevc,aac
BSFS=h264_mp4toannexb,hevc_mp4toannexb,dca_core
ZLIB=
mkdir -p "$BUILD" "$PREFIX"
  CC=${CC:-arm-webos-linux-gnueabi-gcc}
  archive="$BUILD/ffmpeg-$VERSION.tar.xz"
  [ -f "$archive" ] || curl -fsSL "https://ffmpeg.org/releases/ffmpeg-$VERSION.tar.xz" -o "$archive"
  printf '%s  %s\n' "$SHA256" "$archive" | sha256sum -c -
  tar -xf "$archive" -C "$BUILD"
  SRC=.
  cd "$BUILD/ffmpeg-$VERSION"
  # NUVIO_DTS_HOST=1 builds the identical components for fixture tests.
  if [ "${NUVIO_DTS_HOST:-0}" = 1 ]; then
    set -- --cc="$CC" --disable-x86asm
  else
    set -- --enable-cross-compile --arch=arm --target-os=linux \
      --cross-prefix="${CC%-gcc}-" --cc="$CC" --sysroot="${NUVIO_SYSROOT:?SDK sysroot required}"
  fi
# --enable-muxer must come AFTER --disable-everything and --enable-zlib after
# --disable-autodetect (configure applies them in order). Unquoted on purpose:
# both empty for webOS, so its configure line is unchanged.
"$SRC/configure" "$@" --prefix="$PREFIX" --disable-everything ${MUXERS:+--enable-muxer=$MUXERS} \
  --disable-autodetect $ZLIB --disable-shared --enable-static --enable-pic \
  --disable-programs --disable-doc --disable-debug --disable-network \
  --disable-avdevice --disable-avfilter --disable-swscale --disable-postproc \
  --enable-avcodec --enable-avformat --enable-avutil --enable-swresample \
  --enable-decoder="$DECODERS" --enable-encoder=aac,ac3 \
  --enable-demuxer=matroska,mov --enable-parser="$PARSERS" \
  --enable-bsf="$BSFS" \
  --extra-cflags="${CFLAGS:--O2}"
make -j"${NUVIO_DTS_JOBS:-2}"
make install
# Include license text and pin provenance for redistribution of static LGPL libs.
cp "$SRC/COPYING.LGPLv2.1" "$PREFIX/"
printf 'FFmpeg %s\nhttps://ffmpeg.org/releases/ffmpeg-%s.tar.xz\nSHA256 %s\n' "$VERSION" "$VERSION" "$SHA256" > "$PREFIX/SOURCE.txt"
