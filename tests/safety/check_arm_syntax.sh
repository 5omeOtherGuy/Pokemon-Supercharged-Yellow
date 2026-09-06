#!/usr/bin/env bash
# Syntax/type check only. Asset/string macros are deliberately stubbed.
set -euo pipefail
cd "$(dirname "$0")/../.."
sc_generated_engine=${1:?Pass the engine directory of an existing generated build}
arm-none-eabi-gcc -fsyntax-only -iquote engine/include \
  -iquote "$sc_generated_engine/include" -iquote "$sc_generated_engine/include/constants" \
  '-D_(x)=x' '-DCOMPOUND_STRING(x)=((const u8[]){x})' \
  '-DINCBIN_U8(x)={0}' '-DINCBIN_U16(x)={0}' '-DINCBIN_U32(x)={0}' \
  '-DINCGFX_U8(x,y)={0}' '-DINCGFX_U16(x,y)={0}' '-DINCGFX_U32(x,y)={0}' \
  -DMODERN=1 -DTESTING=0 -DFIRERED -std=gnu17 -mthumb -mthumb-interwork \
  -mabi=apcs-gnu -mtune=arm7tdmi -march=armv4t -Werror -Wall \
  -Wno-attributes -Wno-pointer-to-int-cast \
  engine/src/party_menu.c engine/src/daycare.c engine/src/battle_gimmick.c
