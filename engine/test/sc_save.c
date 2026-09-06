#include "global.h"
#include "malloc.h"
#include "save.h"
#include "sc_progression_core.h"
#include "test/test.h"

TEST("SC save: sector validation includes the trainer tail and bounds the ID")
{
    struct SaveSector *sector = AllocZeroed(sizeof(*sector));
    struct SaveSectorLocation locations[NUM_SECTORS_PER_SLOT] = {0};
    struct ScTrainerProgress progress;
    EXPECT(sector != NULL);
    ScInitTrainerProgress(&progress);
    memcpy(sector->saveBlock3Chunk, &progress, sizeof(progress));
    sector->id = 0;
    sector->signature = SECTOR_SIGNATURE;
    EXPECT(ScIsValidSaveSector(sector, locations));
    sector->saveBlock3Chunk[16] ^= 1;
    EXPECT(!ScIsValidSaveSector(sector, locations));
    sector->saveBlock3Chunk[16] ^= 1;
    sector->id = NUM_SECTORS_PER_SLOT;
    EXPECT(!ScIsValidSaveSector(sector, locations));
    sector->id = 65535;
    EXPECT(!ScIsValidSaveSector(sector, locations));
    sector->id = 1;
    EXPECT(ScIsValidSaveSector(sector, locations));
    sector->signature ^= 1;
    EXPECT(!ScIsValidSaveSector(sector, locations));
    Free(sector);
}
