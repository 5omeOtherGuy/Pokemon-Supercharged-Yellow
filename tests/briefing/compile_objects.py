"""Compile both production units through the real game text/ARM pipeline.

Use an already prepared engine tree for generated map headers and preproc only.
No files or processes in that tree are modified.
"""

import argparse
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--generated-engine", type=Path, default=ROOT / "engine")
    args = parser.parse_args()
    shared = args.generated_engine.resolve()
    with tempfile.TemporaryDirectory(prefix="psy-briefing-arm-") as folder:
        temporary = Path(folder)
        for name in ("sc_build_catalog", "sc_briefing"):
            pre = subprocess.run([
                "arm-none-eabi-cpp", "-iquote", "include", "-iquote", str(shared / "include/constants"),
                "-iquote", str(shared / "include"), "-Wno-trigraphs", "-DMODERN=1", "-DTESTING=0",
                "-DFIRERED", "-std=gnu17", f"src/{name}.c"], cwd=ROOT / "engine",
                check=True, stdout=subprocess.PIPE)
            mapped = subprocess.run([
                str(shared / "tools/preproc/preproc"), "-i", "-g", str(temporary / "assets"),
                f"src/{name}.c", "charmap.txt"], cwd=ROOT / "engine", input=pre.stdout,
                check=True, stdout=subprocess.PIPE)
            source = temporary / f"{name}.i"
            source.write_bytes(mapped.stdout)
            subprocess.run([
                "arm-none-eabi-gcc", "-x", "cpp-output", "-mthumb", "-mthumb-interwork", "-O2",
                "-mabi=apcs-gnu", "-mtune=arm7tdmi", "-march=armv4t", "-Wno-pointer-to-int-cast",
                "-std=gnu17", "-Werror", "-Wall", "-Wno-strict-aliasing", "-Wno-attribute-alias",
                "-Woverride-init", "-Wnonnull", "-Wenum-conversion", "-c", str(source),
                "-o", str(temporary / f"{name}.o")], check=True)
            print(f"{name}: real text preprocessing and ARM object compilation passed")


if __name__ == "__main__":
    main()
