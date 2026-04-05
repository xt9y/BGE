#ifndef LEVEL2_H
#define LEVEL2_H

#include "level.h"

static const level_quad_t level2_sector0_quads[] = {
    { .pos = {0.0f, 0.0f, 0.0f}, .rot = {0.0f, 90.0f, 0.0f}, .size = {30.0f, 0.1f}, .tex_idx = 3, .is_solid = true, .is_invisible = true, .color = {1.0f, 0.0f, 0.0f} },
    { .pos = {0.0f, 0.0f, 0.0f}, .rot = {0.0f, 0.0f, 90.0f}, .size = {30.0f, 0.1f}, .tex_idx = 3, .is_solid = true, .is_invisible = true, .color = {0.0f, 1.0f, 0.0f} },
    { .pos = {0.0f, 0.0f, 0.0f}, .rot = {0.0f, 0.0f, 0.0f}, .size = {30.0f, 0.1f}, .tex_idx = 3, .is_solid = true, .is_invisible = true, .color = {0.0f, 0.0f, 1.0f} },
};

static const level_quad_t level2_sector1_quads[] = {
    { .pos = {3.0f, 0.0f, 0.0f}, .rot = {0.0f, 0.0f, 0.0f}, .size = {6.0f, 6.0f}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 0.8f, 0.4f} },
    { .pos = {3.0f,-3.0f, 3.0f}, .rot = {90.0f, 0.0f, 0.0f}, .size = {6.0f, 6.0f}, .tex_idx = 2, .is_solid = true, .is_invisible = false, .color = {0.8f, 1.0f, 0.6f} },
    { .pos = {0.0f, 0.0f, 3.0f}, .rot = {0.0f, 90.0f, 0.0f}, .size = {6.0f, 6.0f}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.8f, 1.0f} },
};

static const level_sector_data_t level2_sectors[] = {
    { .id = 0, .light = {0.6f, 0.6f, 0.6f}, .quads = level2_sector0_quads, .quad_count = sizeof(level2_sector0_quads) / sizeof(level2_sector0_quads[0]) },
    { .id = 1, .light = {0.7f, 0.9f, 1.0f}, .quads = level2_sector1_quads, .quad_count = sizeof(level2_sector1_quads) / sizeof(level2_sector1_quads[0]) },
};

static inline level_data_t load_2(void)
{
    return (level_data_t){
        .name = "Level 2",
        .path = "level2.h",
        .sectors = level2_sectors,
        .sector_count = sizeof(level2_sectors) / sizeof(level2_sectors[0])
    };
}

#endif
