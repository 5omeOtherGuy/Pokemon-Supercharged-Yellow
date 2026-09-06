"""Copy a pinned ROM and replace Mom's script with a disclosed timing fixture.

Creates one level-5 Pikachu (Adamant, male, all IV15) and starts a level-2
Magikarp wild battle. Does not modify the source ROM or a player save. This
tests real presentation callbacks, not ordinary encounter reachability.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    for name in ("rom", "elf", "output"):
        parser.add_argument("--" + name, type=Path, required=True)
    parser.add_argument("--sha256", required=True)
    args = parser.parse_args()
    original = args.rom.read_bytes()
    if hashlib.sha256(original).hexdigest() != args.sha256:
        parser.error("source checksum mismatch")
    symbols = {}
    for line in subprocess.check_output(["arm-none-eabi-nm", "-n", str(args.elf)], text=True).splitlines():
        fields = line.split()
        if len(fields) == 3:
            symbols[fields[2]] = int(fields[0], 16)
    start = symbols["PalletTown_PlayersHouse_1F_EventScript_Mom"]
    end = symbols["PalletTown_PlayersHouse_1F_EventScript_MomOakLookingForYouMale"]
    # Exact encodings from engine/asm/macros/event.inc. The source's actual
    # create-mon routine and normal scripted-wild-battle commands do the work.
    patch = b"\x69\x23" + struct.pack("<I", (symbols["ScrCmd_createmon"] | 1) + 0x02000000)
    patch += struct.pack("<BBHHIHH6H", 0, 6, 25, 5, 0x1f814, 3, 0, *([15] * 6))
    patch += bytes([symbols["SCR_OP_SETWILDBATTLE"]]) + struct.pack("<HBHHBH", 129, 2, 0, 0, 0, 0)
    patch += bytes([symbols["SCR_OP_DOWILDBATTLE"], 0x6b, 0x02])
    offset = start - 0x08000000
    if len(patch) > end - start or offset < 0 or offset + len(patch) > len(original):
        parser.error("fixture exceeds the linked script's extent")
    special = symbols["SPECIAL_ScShowCenterServices"]
    function = struct.unpack_from("<I", original, symbols["gSpecials"] - 0x08000000 + special * 4)[0]
    if function & ~1 != symbols["ScShowCenterServices"] & ~1:
        parser.error("ELF and ROM do not match")
    modified = bytearray(original)
    modified[offset:offset + len(patch)] = patch
    with args.output.open("xb") as out:
        out.write(modified)
    print(json.dumps({"provenance": "diagnostic ROM script fixture; NOT organic progress",
        "source_sha256": args.sha256,
        "elf_sha256": hashlib.sha256(args.elf.read_bytes()).hexdigest(),
        "fixture_sha256": hashlib.sha256(modified).hexdigest(),
        "script_address": hex(start), "original_bytes": original[offset:offset + len(patch)].hex(),
        "fixture_bytes": patch.hex(), "player": "Pikachu5, nature3, male, IV15 each",
        "opponent": "Magikarp2, ordinary scripted-wild traits"}, indent=2))


if __name__ == "__main__":
    main()
