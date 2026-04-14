#ifndef LEVEL2_H
#define LEVEL2_H

#include "level.h"

static level_quad_t level2_sector0_quads[] = {
    { .pos = {0, 0, 0}, .rot = {0, 90, 0}, .size = {30, 0}, .tex_idx = 3, .is_solid = true, .is_invisible = true, .color = {1.0f, 0.0f, 0.0f} },
    { .pos = {0, 0, 0}, .rot = {0, 0, 90}, .size = {30, 0}, .tex_idx = 3, .is_solid = true, .is_invisible = true, .color = {0.0f, 1.0f, 0.0f} },
    { .pos = {0, 0, 0}, .rot = {0, 0, 0}, .size = {30, 0}, .tex_idx = 3, .is_solid = true, .is_invisible = true, .color = {0.0f, 0.0f, 1.0f} },
    { .pos = {16, 3, 12}, .rot = {0, -26, 0}, .size = {8, 6}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
};

static level_quad_t level2_sector1_quads[] = {
    { .pos = {6, -1, 0}, .rot = {0, 10, 0}, .size = {12, 10}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 0.6f, 0.4f} },
    { .pos = {3, -8, 5}, .rot = {90, 0, 0}, .size = {21, 15}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.6f, 0.6f} },
    { .pos = {0, -1, 5}, .rot = {0, 90, 0}, .size = {13, 10}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 0.8f, 0.7f} },
};

static level_sector_data_t level2_sectors[] = {
    { .id = 0, .light = {1.0f, 1.0f, 1.0f}, .quads = level2_sector0_quads, .quad_count = sizeof(level2_sector0_quads) / sizeof(level2_sector0_quads[0]) },
    { .id = 1, .light = {1.0f, 1.0f, 1.0f}, .quads = level2_sector1_quads, .quad_count = sizeof(level2_sector1_quads) / sizeof(level2_sector1_quads[0]) },
};

static inline level_data_t load_2(void)
{
    return (level_data_t){
        .name = "Level 2",
        .path = "level2.h",
        .sectors = level2_sectors,
        .sector_count = sizeof(level2_sectors) / sizeof(level2_sectors[0]),
        .cam_pos = {11.196f, 9.936f, 29.196f},
        .cam_yaw = 267.300f,
        .cam_pitch = -13.700f
    };
}

#endif
