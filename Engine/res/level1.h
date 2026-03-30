#ifndef LEVEL1_H
#define LEVEL1_H

#include "level.h"

static const level_quad_t level1_sector0_quads[] = {
    { .pos = { 10.0f, 0.0f, 1.0f }, .rot = {0.0f, 25.0f, 0.0f}, .size = {3.0f, 0.0f}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {0.0f, 1.0f, 1.0f} },
    { .pos = { 12.0f, 0.0f, -1.0f }, .rot = {0.0f, 0.0f, 0.0f}, .size = {4.0f, 0.0f}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
};

static const level_quad_t level1_sector1_quads[] = {
};

static const level_sector_data_t level1_sectors[] = {
    { .id = 0, .light_intensity = 1.0f, .floor_height = 0.0f, .ceil_height = 3.0f, .quads = level1_sector0_quads, .quad_count = sizeof(level1_sector0_quads) / sizeof(level1_sector0_quads[0]) },
    { .id = 1, .light_intensity = 0.8f, .floor_height = 0.0f, .ceil_height = 3.0f, .quads = level1_sector1_quads, .quad_count = sizeof(level1_sector1_quads) / sizeof(level1_sector1_quads[0]) },
};

static inline level_t load_1(void)
{
    const level_data_t data = {
        .name = "Level 1",
        .path = "level1.h",
        .sectors = level1_sectors,
        .sector_count = sizeof(level1_sectors) / sizeof(level1_sectors[0])
    };
    return level_load(&data);
}

#endif
