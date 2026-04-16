#ifndef LEVEL2_H
#define LEVEL2_H

#include "level.h"

static level_quad_t level2_sector0_quads[] = {
    { .pos = {-7, -1, 16}, .rot = {90, 0, 0}, .size = {21, 15}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.6f, 0.6f}, .sector_id = 0, .portal_id = 0 },
};

static level_quad_t level2_sector1_quads[] = {
    { .pos = {-4, -1, 2}, .rot = {0, 90, 0}, .size = {13, 10}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 0.8f, 0.7f}, .sector_id = 0, .portal_id = 0 },
    { .pos = {-4, -1, 2}, .rot = {0, 10, 0}, .size = {12, 10}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
};

static level_quad_t level2_sector2_quads[] = {
    { .pos = {8, 7, 22}, .rot = {0, 331, 0}, .size = {8, 6}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.5f, 1.0f, 1.0f}, .sector_id = 2, .portal_id = 0 },
};

static level_sector_data_t level2_sectors[] = {
    { .id = 0, .light = {1.0f, 1.0f, 1.0f}, .quads = level2_sector0_quads, .quad_count = sizeof(level2_sector0_quads) / sizeof(level2_sector0_quads[0]) },
    { .id = 1, .light = {0.9f, 0.7f, 1.0f}, .quads = level2_sector1_quads, .quad_count = sizeof(level2_sector1_quads) / sizeof(level2_sector1_quads[0]) },
    { .id = 2, .light = {1.0f, 0.6f, 0.9f}, .quads = level2_sector2_quads, .quad_count = sizeof(level2_sector2_quads) / sizeof(level2_sector2_quads[0]) },
};

static inline level_data_t load_2(void)
{
    return (level_data_t){
        .name = "Level 2",
        .path = "level2.h",
        .sectors = level2_sectors,
        .sector_count = sizeof(level2_sectors) / sizeof(level2_sectors[0]),
        .cam = { .pos = {8.136f, 16.076f, 37.272f}, .yaw = -2254.961f, .pitch = -19.379f }
    };
}

#endif
