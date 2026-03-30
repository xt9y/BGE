#ifndef LEVEL2_H
#define LEVEL2_H

#include "level.h"

static const level_quad_t level2_sector0_quads[] = {
    { .pos = { 10.0f, 0.0f,  1.0f }, .rot = {0.0f, 20.0f, 0.0f}, .size = {3.0f, 3.0f}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 0.0f, 1.0f} },
};

static const level_quad_t level2_sector1_quads[] = {
    { .pos = { 12.0f, 0.0f, -1.0f }, .rot = {0.0f, 80.0f, 0.0f}, .size = {4.0f, 3.0f}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 0.0f, 0.0f} },
};

static const level_sector_data_t level2_sectors[] = {
    { .id = 0, .light_intensity = 1.0f, .floor_height = 0.0f, .ceil_height = 3.0f, .quads = level2_sector0_quads, .quad_count = sizeof(level2_sector0_quads) / sizeof(level2_sector0_quads[0]) },
    { .id = 1, .light_intensity = 0.8f, .floor_height = 0.0f, .ceil_height = 3.0f, .quads = level2_sector1_quads, .quad_count = sizeof(level2_sector1_quads) / sizeof(level2_sector1_quads[0]) },
};

static inline level_t load_2(void)
{
    const level_data_t data = {
        .name = "Level 2",
        .path = "level2.h",
        .sectors = level2_sectors,
        .sector_count = sizeof(level2_sectors) / sizeof(level2_sectors[0])
    };
    return level_load(&data);
}

#endif
