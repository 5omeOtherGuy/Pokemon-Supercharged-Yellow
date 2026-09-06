#!/usr/bin/env bash
# Build only allocated C objects; generated headers/assets are read from a full build.
set -euo pipefail
sc_project=$(cd "$(dirname "$0")/../.." && pwd)
sc_generated_engine=$(realpath "${1:?Pass an existing generated engine directory}")
sc_output=$(mktemp -d /tmp/sc-supply-objects-XXXXXX)
trap 'rm -rf "$sc_output"' EXIT
cd "$sc_generated_engine"
for sc_path in src/sc_supplies_core.c src/sc_supplies.c src/item_use.c src/item_menu.c test/sc_supplies/selection.c test/sc_supplies/status_effect.c; do
  sc_source=$(basename "$sc_path" .c)
  sc_testing=0
  if [[ "$sc_path" == test/* ]]; then sc_testing=1; fi
  arm-none-eabi-cpp -iquote "$sc_project/engine/include" \
    -iquote "$sc_generated_engine/include" -iquote "$sc_generated_engine/include/constants" -iquote "$sc_generated_engine/src" \
    -DMODERN=1 -DTESTING="$sc_testing" -DFIRERED -std=gnu17 "$sc_project/engine/$sc_path" \
    | tools/preproc/preproc -i -g build/assets "$sc_project/engine/$sc_path" charmap.txt \
    > "$sc_output/$sc_source.i"
  arm-none-eabi-gcc -c -x cpp-output "$sc_output/$sc_source.i" -o "$sc_output/$sc_source.o" \
    -mthumb -mthumb-interwork -O2 -mabi=apcs-gnu -mtune=arm7tdmi -march=armv4t \
    -std=gnu17 -fno-short-enums -Werror -Wall -Wno-pointer-to-int-cast -Wno-strict-aliasing \
    -Wno-attribute-alias -Woverride-init -Wnonnull -Wenum-conversion
  echo "Compiled $sc_source.o"
done
