#ifndef LEVEL1_H
#define LEVEL1_H

#include "level.h"

static level_quad_t level1_sector0_quads[] = {
    { .pos = {6, -2, -5}, .rot = {0, 0, 0}, .size = {12, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {6, -8, 1}, .rot = {90, 0, 0}, .size = {12, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-12, -2, 25}, .rot = {0, 90, 0}, .size = {36, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {24, -2, 25}, .rot = {0, 90, 0}, .size = {36, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {6, 0, 1}, .rot = {90, 0, 0}, .size = {12, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {6, -2, 43}, .rot = {0, 0, 0}, .size = {36, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-6, -2, 7}, .rot = {0, 0, 0}, .size = {12, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {0, -2, 1}, .rot = {0, 90, 0}, .size = {12, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {12, -2, 1}, .rot = {0, 90, 0}, .size = {12, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {6, -12, 25}, .rot = {90, 0, 0}, .size = {36, 36}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {6, -20, 25}, .rot = {90, 0, 0}, .size = {36, 36}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {18, -2, 7}, .rot = {0, 0, 0}, .size = {12, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
};

static level_quad_t level1_sector1_quads[] = {
};

static level_sector_data_t level1_sectors[] = {
    { .id = 0, .light = {0.5f, 0.5f, 0.5f}, .quads = level1_sector0_quads, .quad_count = sizeof(level1_sector0_quads) / sizeof(level1_sector0_quads[0]) },
    { .id = 1, .light = {1.0f, 1.0f, 1.0f}, .quads = level1_sector1_quads, .quad_count = sizeof(level1_sector1_quads) / sizeof(level1_sector1_quads[0]) },
};

static inline level_data_t load_1(void)
{
    return (level_data_t){
        .name = "Level 1",
        .path = "level1.h",
        .sectors = level1_sectors,
        .sector_count = sizeof(level1_sectors) / sizeof(level1_sectors[0]),
        .cam_pos = {5.082f, 2.561f, -1.545f},
        .cam_yaw = -636.921f,
        .cam_pitch = -13.185f
    };
}

#endif
