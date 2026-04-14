#ifndef LEVEL1_H
#define LEVEL1_H

#include "level.h"

static level_quad_t level1_sector0_quads[] = {
    { .pos = {0, 0, 1}, .rot = {90, 0, 0}, .size = {21, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {0, 0, 1}, .rot = {0, 0, 0}, .size = {33, 2}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f} },
    { .pos = {12, -8, -11}, .rot = {0, 33, 0}, .size = {12, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-3, 4, 19}, .rot = {0, 90, 90}, .size = {12, 30}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f} },
    { .pos = {27, 2, 19}, .rot = {90, 327, 0}, .size = {10, 22}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {33, -10, 1}, .rot = {0, 90, 0}, .size = {30, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-3, -10, 55}, .rot = {0, 0, 0}, .size = {25, 12}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.4f, 0.2f, 0.2f} },
    { .pos = {-1, -10, 3}, .rot = {50, 0, 0}, .size = {22, 3}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f} },
    { .pos = {0, -8, 1}, .rot = {90, 0, 0}, .size = {21, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {0, -8, -11}, .rot = {0, 0, 0}, .size = {12, 8}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {0, -8, -11}, .rot = {0, 90, 0}, .size = {12, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {0, -10, 1}, .rot = {0, 106, 0}, .size = {11, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {21, -8, 1}, .rot = {0, 0, 0}, .size = {12, 8}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f} },
    { .pos = {22, -10, 41}, .rot = {0, 90, 0}, .size = {14, 12}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {0.8f, 0.6f, 0.6f} },
    { .pos = {22, 1, 19}, .rot = {0, 90, 0}, .size = {12, 1}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {26, 0, 33}, .rot = {0, 90, 90}, .size = {8, 8}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f} },
    { .pos = {20, -10, 19}, .rot = {0, 90, 59}, .size = {12, 6}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f} },
    { .pos = {-3, 1, 32}, .rot = {0, 0, 0}, .size = {23, 3}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {21, -8, -5}, .rot = {0, 90, 0}, .size = {6, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {22, -11, 19}, .rot = {0, 0, 0}, .size = {12, 13}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-3, -10, 1}, .rot = {0, 90, 0}, .size = {4, 12}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {22, -10, 31}, .rot = {0, 0, 0}, .size = {12, 12}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {22, -10, 17}, .rot = {0, 90, 0}, .size = {2, 12}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-3, -10, 11}, .rot = {0, 90, 0}, .size = {44, 21}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-5, -10, 23}, .rot = {0, 0, 0}, .size = {2, 21}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-5, 2, 11}, .rot = {90, 0, 0}, .size = {2, 6}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-5, 11, 23}, .rot = {90, 0, 0}, .size = {2, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-5, -10, 11}, .rot = {0, 0, 0}, .size = {2, 21}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-5, -10, 5}, .rot = {0, 0, 0}, .size = {2, 12}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-3, -10, 1}, .rot = {0, 0, 0}, .size = {3, 12}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {0, -10, 1}, .rot = {0, 0, 0}, .size = {33, 2}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f} },
    { .pos = {-3, 1, 19}, .rot = {0, 90, 90}, .size = {12, 2}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {22, -10, 1}, .rot = {0, 90, 0}, .size = {2, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-3, -10, 19}, .rot = {0, 0, 0}, .size = {2, 11}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-1, -10, 1}, .rot = {0, 0, 0}, .size = {2, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {20, -10, 32}, .rot = {0, 0, 0}, .size = {2, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {20, -10, 19}, .rot = {0, 0, 0}, .size = {2, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {20, -10, 31}, .rot = {0, 90, 0}, .size = {1, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {20, -10, 18}, .rot = {0, 0, 0}, .size = {2, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {1, -11, 1}, .rot = {0, 90, 0}, .size = {2, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-1, -10, 31}, .rot = {0, 90, 0}, .size = {1, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-1, 0, 3}, .rot = {0, 0, 0}, .size = {22, 2}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-1, 0, 3}, .rot = {90, 0, 0}, .size = {22, 2}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {25, 1, 19}, .rot = {0, 90, 0}, .size = {12, 1}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f} },
    { .pos = {25, -7, 19}, .rot = {0, 90, 90}, .size = {12, 8}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f} },
    { .pos = {26, 0, 33}, .rot = {0, 90, 331}, .size = {8, 4}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f} },
    { .pos = {-3, 2, 55}, .rot = {90, 0, 0}, .size = {30, 24}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-3, 2, 19}, .rot = {90, 0, 0}, .size = {30, 18}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {22, -10, 33}, .rot = {0, 0, 0}, .size = {12, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {22, -10, 31}, .rot = {0, 90, 0}, .size = {2, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-3, -10, -1}, .rot = {270, 0, 0}, .size = {38, 56}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {22, -10, 41}, .rot = {0, 0, 0}, .size = {12, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {0.8f, 0.6f, 0.6f} },
    { .pos = {22, 2, 19}, .rot = {0, 90, 90}, .size = {12, 8}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f} },
    { .pos = {-1, 1, 19}, .rot = {0, 90, 0}, .size = {12, 3}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-5, -10, 5}, .rot = {0, 90, 0}, .size = {18, 21}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {3, -6, 50}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_idx = -1, .is_solid = false, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {15, -6, 50}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_idx = 0, .is_solid = false, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {12, -6, 50}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_idx = 1, .is_solid = false, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {9, -6, 50}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_idx = 2, .is_solid = false, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {6, -6, 50}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_idx = 3, .is_solid = false, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {20, -10, 31}, .rot = {0, 0, 0}, .size = {2, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-3, -10, 31}, .rot = {0, 0, 0}, .size = {2, 11}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-3, -10, 32}, .rot = {0, 0, 0}, .size = {2, 11}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-3, -10, 18}, .rot = {0, 0, 0}, .size = {2, 11}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {20, -10, 18}, .rot = {0, 90, 0}, .size = {1, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-1, -10, 18}, .rot = {0, 90, 0}, .size = {1, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {22, -10, 17}, .rot = {0, 0, 0}, .size = {12, 12}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {-3, 1, 18}, .rot = {0, 0, 0}, .size = {23, 3}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-1, 1, 32}, .rot = {90, 90, 90}, .size = {21, 1}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {20, 1, 19}, .rot = {0, 90, 0}, .size = {12, 3}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {20, 1, 19}, .rot = {0, 90, 90}, .size = {12, 2}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-1, 1, 19}, .rot = {90, 90, 90}, .size = {21, 1}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-3, 1, 19}, .rot = {0, 0, 0}, .size = {23, 3}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-3, 1, 31}, .rot = {0, 0, 0}, .size = {23, 3}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {25, 1, 19}, .rot = {0, 90, 90}, .size = {12, 8}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f} },
    { .pos = {21, -10, 1}, .rot = {0, 90, 0}, .size = {2, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {21, -10, 3}, .rot = {0, 0, 0}, .size = {1, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
    { .pos = {-1, -10, 3}, .rot = {0, 0, 0}, .size = {2, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f} },
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
        .cam = { .pos = {9.167f, -3.500f, -4.535f}, .yaw = -2794.394f, .pitch = 2.600f }
    };
}

#endif
