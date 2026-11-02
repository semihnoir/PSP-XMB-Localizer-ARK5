#!/usr/bin/env bash
# Builds localizer.prx (kernel loader + embedded user module) with the pspdev toolchain.
#
#   ./build.sh          silent release build   -> dist/localizer.prx
#   ./build.sh debug    adds ms0:/seplugins/localizer.log and localizer_k.log -> dist/localizer_debug.prx
#
# Requires pspdev (https://pspdev.github.io) with psp-gcc, bin2c and psp-build-exports in PATH or in $PSPDEV/bin.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
export PSPDEV="${PSPDEV:-$HOME/pspdev}"
export PATH="$PATH:$PSPDEV/bin"

MODE="${1:-release}"
FLAGS="${LOC_FLAGS:-}"; OUT="${LOC_OUT:-localizer.prx}"
if [ "$MODE" = "debug" ]; then FLAGS="$FLAGS -DLOC_DEBUG"; OUT="${LOC_OUT:-localizer_debug.prx}"; fi

WORK="$ROOT/build"
rm -rf "$WORK"; mkdir -p "$WORK" "$ROOT/dist"
cp -r "$ROOT/src/." "$WORK/"
cp "$ROOT/assets/genesis.png" "$ROOT/assets/semihnoir.png" "$WORK/"
cd "$WORK"

# the user module is built first and embedded into the kernel module
bin2c genesis.png    user/genesis.h    genesis
bin2c semihnoir.png  user/semihnoir.h  semihnoir
sed -i "s/^CFLAGS = .*/& $FLAGS/" user/Makefile kernel/Makefile
make -C user -f Makefile all
bin2c user/localizeruser.prx kernel/localize.h localize
sed -i 's#((aligned(16)))#((aligned(64)))#g; s#static##g' kernel/localize.h
make -C kernel -f Makefile all

cp kernel/localizer.prx "$ROOT/dist/$OUT"
echo "Built dist/$OUT"
