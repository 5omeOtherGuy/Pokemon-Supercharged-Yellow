"""Patch only a disposable ROM copy to call the briefing from Mom's interaction.

This does not establish legitimate boss reachability or campaign progress. The
original ROM, ELF and any original cartridge save are never modified.
"""
import argparse
import hashlib
import json
from pathlib import Path
import struct
import subprocess


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, required=True)
    parser.add_argument("--elf", type=Path, required=True)
    parser.add_argument("--sha256", required=True)
    parser.add_argument("--trainer-id", type=int, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    if not 0 < args.trainer_id < 65536:
        parser.error("trainer ID must fit a nonzero u16")
    original = args.rom.read_bytes()
    if hashlib.sha256(original).hexdigest() != args.sha256:
        parser.error("source ROM SHA-256 does not match")
    if args.output.exists():
        parser.error("output must be a new disposable path")
    listing = subprocess.check_output(["arm-none-eabi-nm", "-n", str(args.elf)], text=True)
    symbols = {}
    for line in listing.splitlines():
        fields = line.split()
        if len(fields) == 3:
            symbols[fields[2]] = int(fields[0], 16)
    script = symbols["PalletTown_PlayersHouse_1F_EventScript_Mom"]
    special = symbols["SPECIAL_ScShowBossBriefing"]
    table_offset = symbols["gSpecials"] - 0x08000000 + special * 4
    function = struct.unpack_from("<I", original, table_offset)[0]
    if function & ~1 != symbols["ScShowBossBriefing"] & ~1:
        parser.error("ELF special/function addresses do not match this ROM")
    # Source: engine/data/script_cmd_table.inc, engine/asm/macros/event.inc.
    # lockall; setvar VAR_0x8004, trainer; special; waitstate; releaseall; end.
    patch = b"\x69\x16" + struct.pack("<HH", 0x8004, args.trainer_id)
    patch += b"\x25" + struct.pack("<H", special) + b"\x27\x6b\x02"
    offset = script - 0x08000000
    next_script = symbols["PalletTown_PlayersHouse_1F_EventScript_MomOakLookingForYouMale"]
    if offset < 0 or next_script - script < len(patch) or offset + len(patch) > len(original):
        parser.error("script patch does not fit the exact linked target")
    modified = bytearray(original)
    modified[offset:offset + len(patch)] = patch
    with args.output.open("xb") as output:
        output.write(modified)
    print(json.dumps({
        "provenance": "diagnostic-rom-script-fixture; NOT organic campaign progress",
        "source_rom": str(args.rom.resolve()), "source_sha256": args.sha256,
        "elf": str(args.elf.resolve()), "elf_sha256": hashlib.sha256(args.elf.read_bytes()).hexdigest(),
        "output": str(args.output.resolve()), "output_sha256": hashlib.sha256(modified).hexdigest(),
        "trainer_id": args.trainer_id, "script_address": hex(script), "special_index": special,
        "original_bytes": original[offset:offset + len(patch)].hex(), "fixture_bytes": patch.hex(),
    }, indent=2))


if __name__ == "__main__":
    main()
