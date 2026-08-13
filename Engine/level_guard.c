#include "level.h"

void level_guard_runtime_ids(level_data_t* level)
{
    if (!level || !level->sectors) return;

    for (i32 s = 0; s < level->sector_count; ++s) {
        level_sector_data_t* sector = &level->sectors[s];
        if (!sector->quads) continue;

        for (i32 q = 0; q < sector->quad_count; ++q) {
            if (sector->quads[q].portal_id < 0 || sector->quads[q].portal_id > 255)
                sector->quads[q].portal_id = 0;
        }
    }
}
