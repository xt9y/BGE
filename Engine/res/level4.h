#ifndef LEVEL4_H
#define LEVEL4_H

#include "level.h"

static const level_quad_t level4_sector0_quads[] = {
    { .pos = {10.0f, 0.0f, 1.0f}, .rot = {0.0f, 20.0f, 0.0f}, .size = {3.0f, 3.0f}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 0.0f, 0.0f} },
};

static const level_quad_t level4_sector1_quads[] = {
    { .pos = {12.0f, 0.0f, -1.0f}, .rot = {0.0f, 80.0f, 0.0f}, .size = {4.0f, 3.0f}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 0.0f, 0.0f} },
};

static const level_sector_data_t level4_sectors[] = {
    { .id = 0, .light_intensity = 1.0f, .quads = level4_sector0_quads, .quad_count = sizeof(level4_sector0_quads) / sizeof(level4_sector0_quads[0]) },
    { .id = 1, .light_intensity = 0.1f, .quads = level4_sector1_quads, .quad_count = sizeof(level4_sector1_quads) / sizeof(level4_sector1_quads[0]) },
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

