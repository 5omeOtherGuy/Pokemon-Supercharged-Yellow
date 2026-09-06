#!/usr/bin/env bash
# Build only allocated C objects; generated headers/assets are read from a full build.
set -euo pipefail
sc_project=$(cd "$(dirname "$0")/../.." && pwd)
sc_generated_engine=$(realpath "${1:?Pass an existing generated engine directory}")
sc_output=$(mktemp -d /tmp/sc-supply-objects-XXXXXX)
trap 'rm -rf "$sc_output"' EXIT
cd "$sc_generated_engine"
for sc_source in sc_supplies_core sc_supplies item_use item_menu; do
  arm-none-eabi-cpp -iquote "$sc_project/engine/include" \
    -iquote "$sc_generated_engine/include" -iquote "$sc_generated_engine/include/constants" -iquote "$sc_generated_engine/src" \
    -DMODERN=1 -DTESTING=0 -DFIRERED -std=gnu17 "$sc_project/engine/src/$sc_source.c" \
    | tools/preproc/preproc -i -g build/assets "$sc_project/engine/src/$sc_source.c" charmap.txt \
    > "$sc_output/$sc_source.i"
  arm-none-eabi-gcc -c -x cpp-output "$sc_output/$sc_source.i" -o "$sc_output/$sc_source.o" \
    -mthumb -mthumb-interwork -O2 -mabi=apcs-gnu -mtune=arm7tdmi -march=armv4t \
    -std=gnu17 -fno-short-enums -Werror -Wall -Wno-pointer-to-int-cast -Wno-strict-aliasing \
    -Wno-attribute-alias -Woverride-init -Wnonnull -Wenum-conversion
  echo "Compiled $sc_source.o"
done
