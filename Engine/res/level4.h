#ifndef LEVEL4_H
#define LEVEL4_H

#include "level.h"

static const level_quad_t level4_sector0_quads[] = {
    { .pos = {0.0f, 0.0f, 0.0f}, .rot = {0.0f, 90.0f, 0.0f}, .size = {30.0f, 0.1f}, .tex_idx = 3, .is_solid = true, .is_invisible = true, .color = {1.0f, 0.0f, 0.0f} },
    { .pos = {0.0f, 0.0f, 0.0f}, .rot = {0.0f, 0.0f, 90.0f}, .size = {30.0f, 0.1f}, .tex_idx = 3, .is_solid = true, .is_invisible = true, .color = {0.0f, 1.0f, 0.0f} },
    { .pos = {0.0f, 0.0f, 0.0f}, .rot = {0.0f, 0.0f, 0.0f}, .size = {30.0f, 0.1f}, .tex_idx = 3, .is_solid = true, .is_invisible = true, .color = {0.0f, 0.0f, 1.0f} },
};

static const level_quad_t level4_sector1_quads[] = {
    { .pos = {3.0f, 0.0f, 0.0f}, .rot = {0.0f, 0.0f, 0.0f}, .size = {6.0f, 6.0f}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {3.0f,-3.0f, 3.0f}, .rot = {90.0f, 0.0f, 0.0f}, .size = {6.0f, 6.0f}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {0.0f, 0.0f, 3.0f}, .rot = {0.0f, 90.0f, 0.0f}, .size = {6.0f, 6.0f}, .tex_idx = 2, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
};

static const level_sector_data_t level4_sectors[] = {
    { .id = 0, .light = {0.6f, 0.6f, 0.6f}, .quads = level4_sector0_quads, .quad_count = sizeof(level4_sector0_quads) / sizeof(level4_sector0_quads[0]) },
    { .id = 1, .light = {1.0f, 0.8f, 0.5f}, .quads = level4_sector1_quads, .quad_count = sizeof(level4_sector1_quads) / sizeof(level4_sector1_quads[0]) },
};

static inline level_data_t load_4(void)
{
    return (level_data_t){
        .name = "Level 4",
        .path = "level4.h",
        .sectors = level4_sectors,
        .sector_count = sizeof(level4_sectors) / sizeof(level4_sectors[0])
    };
}

#endif
