#ifndef LEVEL1_H
#define LEVEL1_H

#include "level.h"

static level_quad_t level1_sector0_quads[] = {
    { .pos = {0, 0, 1}, .rot = {90, 0, 0}, .size = {21, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {0, 0, 1}, .rot = {0, 0, 0}, .size = {33, 2}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {12, -8, -11}, .rot = {0, 33, 0}, .size = {12, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {21, -8, -11}, .rot = {0, 90, 0}, .size = {12, 8}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-3, 2, 23}, .rot = {90, 0, 0}, .size = {31, 22}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {0, -10, 1}, .rot = {0, 107, 0}, .size = {11, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 0.8f} },
    { .pos = {21, -8, 1}, .rot = {0, 0, 0}, .size = {12, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {0, -10, 1}, .rot = {0, 0, 0}, .size = {33, 2}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {0, -8, 1}, .rot = {90, 0, 0}, .size = {21, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {0, -8, -11}, .rot = {0, 0, 0}, .size = {12, 8}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {0, -8, -11}, .rot = {0, 90, 0}, .size = {12, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-5, -10, 24}, .rot = {90, 0, 0}, .size = {37, 23}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-3, -10, 11}, .rot = {0, 90, 0}, .size = {12, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-16, -12, 29}, .rot = {0, 90, 0}, .size = {12, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-16, -12, 55}, .rot = {0, 90, 0}, .size = {12, 12}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-16, -12, 68}, .rot = {0, 90, 0}, .size = {12, 12}, .tex_idx = 2, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-16, -12, 81}, .rot = {0, 90, 0}, .size = {12, 12}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-16, 1, 42}, .rot = {0, 90, 0}, .size = {12, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-16, -12, 42}, .rot = {0, 90, 0}, .size = {12, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
};

static level_quad_t level1_sector1_quads[] = {
};

static level_sector_data_t level1_sectors[] = {
    { .id = 0, .light = {1.0f, 1.0f, 1.0f}, .quads = level1_sector0_quads, .quad_count = sizeof(level1_sector0_quads) / sizeof(level1_sector0_quads[0]) },
    { .id = 1, .light = {1.0f, 1.0f, 1.0f}, .quads = level1_sector1_quads, .quad_count = sizeof(level1_sector1_quads) / sizeof(level1_sector1_quads[0]) },
};

static inline level_data_t load_1(void)
{
    return (level_data_t){
        .name = "Level 1",
        .path = "level1.h",
        .sectors = level1_sectors,
        .sector_count = sizeof(level1_sectors) / sizeof(level1_sectors[0]),
        .cam_pos = {15.437f, -4.992f, 46.820f},
        .cam_yaw = -899.863f,
        .cam_pitch = 16.311f
    };
}

#endif
