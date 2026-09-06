"""Compile service production units through the real game text/ARM pipeline.

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
    with tempfile.TemporaryDirectory(prefix="psy-services-arm-") as folder:
        temporary = Path(folder)
        for filename in ("src/sc_services_model.c", "src/sc_services.c", "test/sc_services/transactions.c"):
            name = Path(filename).stem
            testing = int(filename.startswith("test/"))
            pre = subprocess.run([
                "arm-none-eabi-cpp", "-iquote", "include", "-iquote", str(shared / "include/constants"),
                "-iquote", str(shared / "include"), "-Wno-trigraphs", "-DMODERN=1", f"-DTESTING={testing}",
                "-DFIRERED", "-std=gnu17", filename], cwd=ROOT / "engine",
                check=True, stdout=subprocess.PIPE)
            mapped = subprocess.run([
                str(shared / "tools/preproc/preproc"), "-i", "-g", str(temporary / "assets"),
                filename, "charmap.txt"], cwd=ROOT / "engine", input=pre.stdout,
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
