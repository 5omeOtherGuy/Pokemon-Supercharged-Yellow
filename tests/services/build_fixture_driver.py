"""Build a separate mGBA diagnostic driver; never modify the ordinary driver.

Adds explicit aligned EWRAM/IWRAM fixture32 writes. Every later status marks the
session diagnostic-state-injected. Use only isolated test ROMs and test saves.
"""
from pathlib import Path
import subprocess

ROOT=Path(__file__).resolve().parents[2]
OUT=ROOT/'build/services'
OUT.mkdir(parents=True,exist_ok=True)
source=(ROOT/'tools/emulator/headless.c').read_text()
marker='        } else if (!strcmp(line, "status")) {'
assert source.count(marker)==1
insert='''        } else if (!strncmp(line, "fixture32 ", 10)) {
            if (!pair(line + 10, &a, &b) || (a & 3) ||
                !((a >= 0x02000000 && a <= 0x0203fffc) ||
                  (a >= 0x03000000 && a <= 0x03007ffc))) {
                error("fixture32 requires aligned emulated work-RAM address and u32");
                continue;
            }
            provenance = "diagnostic-state-injected";
            core->busWrite32(core, a, b);
            status();
'''
source=source.replace(marker,insert+marker)
source=source.replace('fresh-input-only','diagnostic-rom-fresh-save').replace('existing-save-unverified','diagnostic-rom-existing-save')
source=source.replace('/* No cheats, savestates, memory writes, personal config, or implicit save paths. */','/* GENERATED DIAGNOSTIC DRIVER: explicit work-RAM injection; not organic gameplay evidence. */')
(OUT/'fixture_driver.c').write_text(source)
subprocess.run(['cc','-std=c11','-Wall','-Wextra','-Werror','-O2','-o',str(OUT/'fixture-driver'),str(OUT/'fixture_driver.c'),'-lmgba'],check=True)
print(OUT/'fixture-driver')
