#!/usr/bin/env python3
"""Build a private owner test ROM, optionally prove a clean source rebuild.

Artifacts stay in ignored dist/. Never upload this ROM/ZIP to a public release.
"""
import argparse
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import tarfile
import tempfile
import zipfile
from urllib.parse import quote

ROOT = Path(__file__).resolve().parents[1]
ROM_NAME = "Pokemon-Supercharged-Yellow-playtest.gba"
GUIDES = {"docs/player-playtest.md": "START-HERE.md", "docs/testing-cheats.md": "PC-TEST-TOOLS.md",
          "docs/validation/owner-playtest.md": "VALIDATION.md"}


def command(*args, cwd=ROOT):
    return subprocess.check_output(args, cwd=cwd, text=True).strip()


def sha256(path):
    with path.open("rb") as source:
        return hashlib.file_digest(source, "sha256").hexdigest()


def check_rom(path):
    data = path.read_bytes()
    if not 1024 * 1024 <= len(data) <= 32 * 1024 * 1024:
        raise RuntimeError("Unexpected GBA ROM size")
    if data[0xB2] != 0x96 or (sum(data[0xA0:0xBD]) + data[0xBD] + 0x19) & 255:
        raise RuntimeError("Invalid GBA header or complement checksum")
    return {"sha256": hashlib.sha256(data).hexdigest(), "bytes": len(data)}


def build(checkout, jobs, log):
    with log.open("w") as output:
        subprocess.run(["make", "-C", str(checkout / "engine"), "BUILD=firered", "SC_TEST_TOOLS=1", "-j" + str(jobs)],
                       stdout=output, stderr=subprocess.STDOUT, check=True)
    rom = checkout / "engine/pokefirered-playtest.gba"
    check_rom(rom)
    return rom


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--jobs", type=int, default=2, choices=range(1, 9))
    parser.add_argument("--repro-check", action="store_true", help="build a fresh git archive and require identical ROM bytes")
    parser.add_argument("--output", type=Path, default=ROOT / "dist/owner-playtest")
    args = parser.parse_args()
    if command("git", "status", "--porcelain", "--untracked-files=no"):
        parser.error("Commit tracked changes first so the recorded source revision is exact")
    compiler = command("arm-none-eabi-gcc", "-dumpfullversion")
    if compiler != "13.2.1":
        parser.error("Reference compiler is ARM GCC 13.2.1; found " + compiler)
    revision = command("git", "rev-parse", "HEAD")
    logs = ROOT / "build/playtest-package"
    logs.mkdir(parents=True, exist_ok=True)
    print("Building owner test ROM; log: " + str(logs / "build.log"), flush=True)
    rom = build(ROOT, args.jobs, logs / "build.log")
    identity = check_rom(rom)
    if args.repro_check:
        print("Checking a clean source rebuild; log: " + str(logs / "rebuild.log"), flush=True)
        with tempfile.TemporaryDirectory(prefix="sc-playtest-repro-", dir=ROOT / "build") as directory:
            clean = Path(directory)
            archive = clean / "source.tar"
            subprocess.run(["git", "archive", "--format=tar", "--output=" + str(archive), revision], cwd=ROOT, check=True)
            with tarfile.open(archive) as source:
                source.extractall(clean, filter="data")
            archive.unlink()
            rebuilt = build(clean, args.jobs, logs / "rebuild.log")
            if sha256(rebuilt) != identity["sha256"]:
                shutil.copy2(rebuilt, logs / "different-rebuild.gba")
                raise RuntimeError("Clean rebuild differs; retained comparison ROM in build/playtest-package")
    if command("git", "rev-parse", "HEAD") != revision or command("git", "status", "--porcelain", "--untracked-files=no"):
        raise RuntimeError("Source changed during build; artifact was not packaged")
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    # Refuse to overwrite a different owner build silently.
    target = output / ROM_NAME
    if target.exists() and sha256(target) != identity["sha256"]:
        raise RuntimeError("Output contains another ROM; choose a new --output directory")
    shutil.copy2(rom, target)
    manifest = {
        "kind": "private-owner-playtest", "source_commit": revision,
        "engine_tree": command("git", "rev-parse", revision + ":engine"),
        "rom": ROM_NAME, **identity, "SC_TEST_TOOLS": 1, "switches_default": "off",
        "compiler": command("arm-none-eabi-gcc", "--version").splitlines()[0],
        "linker": command("arm-none-eabi-ld", "--version").splitlines()[0],
        "clean_source_rebuild_identical": args.repro_check,
        "validation": "See docs/validation/owner-playtest.md at source_commit; full campaign/human acceptance incomplete",
        "distribution": "Local ROM for owner testing; do not upload ROM or this ZIP to public releases",
    }
    (output / "BUILD-MANIFEST.json").write_text(json.dumps(manifest, indent=2) + "\n")
    for source, name in GUIDES.items():
        source_path = ROOT / source
        def link(match):
            target = match[1]
            if "://" in target or target.startswith("#"):
                return match[0]
            filename, separator, anchor = target.partition("#")
            relative = (source_path.parent / filename).resolve().relative_to(ROOT).as_posix()
            destination = GUIDES.get(relative, "https://github.com/5omeOtherGuy/Pokemon-Supercharged-Yellow/blob/" + revision + "/" + quote(relative))
            return "](" + destination + separator + anchor + ")"
        (output / name).write_text(re.sub(r"\]\(([^)]+)\)", link, source_path.read_text()))
    names = [ROM_NAME, "BUILD-MANIFEST.json", "START-HERE.md", "PC-TEST-TOOLS.md", "VALIDATION.md"]
    with zipfile.ZipFile(output / "Supercharged-Yellow-owner-playtest.zip", "w", compression=zipfile.ZIP_DEFLATED) as bundle:
        for name in names:
            info = zipfile.ZipInfo(name, date_time=(1980, 1, 1, 0, 0, 0))
            info.compress_type = zipfile.ZIP_DEFLATED
            info.external_attr = 0o644 << 16
            bundle.writestr(info, (output / name).read_bytes())
    (output / "SHA256SUMS").write_text("".join(f"{sha256(output / name)}  {name}\n" for name in names + ["Supercharged-Yellow-owner-playtest.zip"]))
    print(json.dumps({"output": str(output), **manifest}, indent=2))


if __name__ == "__main__":
    main()
