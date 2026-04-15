#ifndef LEVEL1_H
#define LEVEL1_H

#include "level.h"

static level_quad_t level1_sector0_quads[] = {
    { .pos = {-3, 4, 19}, .rot = {0, 90, 90}, .size = {12, 30}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f}, .sector_id = 0 },
    { .pos = {27, 2, 19}, .rot = {90, 327, 0}, .size = {8, 18}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {33, -10, 1}, .rot = {0, 90, 0}, .size = {30, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-3, -10, 55}, .rot = {0, 0, 0}, .size = {25, 12}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.4f, 0.2f, 0.2f}, .sector_id = 0 },
    { .pos = {-1, -8, 1}, .rot = {225, 0, 0}, .size = {22, 3}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f}, .sector_id = 0 },
    { .pos = {0, -10, 1}, .rot = {0, 106, 0}, .size = {11, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {22, -10, 41}, .rot = {0, 90, 0}, .size = {14, 12}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {0.8f, 0.6f, 0.6f}, .sector_id = 0 },
    { .pos = {26, 0, 33}, .rot = {0, 90, 90}, .size = {8, 8}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f}, .sector_id = 0 },
    { .pos = {-3, 1, 32}, .rot = {0, 0, 0}, .size = {23, 3}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-3, -10, 1}, .rot = {0, 90, 0}, .size = {4, 12}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {22, -10, 17}, .rot = {0, 90, 0}, .size = {2, 12}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {-3, -10, 11}, .rot = {0, 90, 0}, .size = {44, 21}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {-5, -10, 23}, .rot = {0, 0, 0}, .size = {2, 21}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {-5, 2, 11}, .rot = {90, 0, 0}, .size = {2, 6}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {-5, 11, 23}, .rot = {90, 0, 0}, .size = {2, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {-5, -10, 11}, .rot = {0, 0, 0}, .size = {2, 21}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {-5, -10, 5}, .rot = {0, 0, 0}, .size = {2, 12}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {-3, -10, 1}, .rot = {0, 0, 0}, .size = {3, 12}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {-3, 1, 19}, .rot = {0, 90, 90}, .size = {12, 2}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {22, -10, 1}, .rot = {0, 90, 0}, .size = {2, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-3, -10, 19}, .rot = {0, 0, 0}, .size = {2, 11}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-1, -10, 1}, .rot = {0, 0, 0}, .size = {2, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {20, -10, 32}, .rot = {0, 0, 0}, .size = {2, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {20, -10, 19}, .rot = {0, 0, 0}, .size = {2, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {20, -10, 31}, .rot = {0, 90, 0}, .size = {1, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {20, -10, 18}, .rot = {0, 0, 0}, .size = {2, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {1, -11, 1}, .rot = {0, 90, 0}, .size = {2, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-1, -10, 31}, .rot = {0, 90, 0}, .size = {1, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-1, 0, 3}, .rot = {0, 0, 0}, .size = {22, 2}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-1, 0, 3}, .rot = {90, 0, 0}, .size = {22, 2}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {26, 0, 33}, .rot = {0, 90, 331}, .size = {8, 4}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.1f, 0.1f, 0.1f}, .sector_id = 0 },
    { .pos = {-3, 2, 55}, .rot = {90, 0, 0}, .size = {30, 24}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {-3, 2, 19}, .rot = {90, 0, 0}, .size = {30, 18}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {22, -10, 33}, .rot = {0, 0, 0}, .size = {12, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {22, -10, 31}, .rot = {0, 90, 0}, .size = {2, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {-3, -10, -1}, .rot = {270, 0, 0}, .size = {38, 56}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {22, -10, 41}, .rot = {0, 0, 0}, .size = {12, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {0.8f, 0.6f, 0.6f}, .sector_id = 0 },
    { .pos = {-1, 1, 19}, .rot = {0, 90, 0}, .size = {12, 3}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-5, -10, 5}, .rot = {0, 90, 0}, .size = {18, 21}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {20, -10, 31}, .rot = {0, 0, 0}, .size = {2, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-3, -10, 31}, .rot = {0, 0, 0}, .size = {2, 11}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-3, -10, 32}, .rot = {0, 0, 0}, .size = {2, 11}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-3, -10, 18}, .rot = {0, 0, 0}, .size = {2, 11}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {20, -10, 18}, .rot = {0, 90, 0}, .size = {1, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-1, -10, 18}, .rot = {0, 90, 0}, .size = {1, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {22, -10, 17}, .rot = {0, 0, 0}, .size = {12, 12}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {-3, 1, 18}, .rot = {0, 0, 0}, .size = {23, 3}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-1, 1, 32}, .rot = {90, 90, 90}, .size = {21, 1}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {20, 1, 19}, .rot = {0, 90, 0}, .size = {12, 3}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {20, 1, 19}, .rot = {0, 90, 90}, .size = {12, 2}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-1, 1, 19}, .rot = {90, 90, 90}, .size = {21, 1}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-3, 1, 19}, .rot = {0, 0, 0}, .size = {23, 3}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-3, 1, 31}, .rot = {0, 0, 0}, .size = {23, 3}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {21, -10, 1}, .rot = {0, 90, 0}, .size = {2, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {21, -10, 3}, .rot = {0, 0, 0}, .size = {1, 12}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {-1, -10, 3}, .rot = {0, 0, 0}, .size = {2, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
    { .pos = {21, -10, 1}, .rot = {0, 0, 0}, .size = {12, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0 },
    { .pos = {22, 1, 19}, .rot = {0, 90, 0}, .size = {12, 1}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 0 },
};

static level_quad_t level1_sector1_quads[] = {
    { .pos = {0, -8, 1}, .rot = {90, 0, 0}, .size = {21, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1 },
    { .pos = {0, -8, -11}, .rot = {0, 90, 0}, .size = {12, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1 },
    { .pos = {0, -8, -11}, .rot = {0, 0, 0}, .size = {12, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1 },
    { .pos = {12, -8, -11}, .rot = {0, 41, 0}, .size = {12, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1 },
    { .pos = {21, -8, -5}, .rot = {0, 90, 0}, .size = {6, 8}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1 },
    { .pos = {0, 0, 1}, .rot = {90, 0, 0}, .size = {21, 12}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1 },
    { .pos = {22, -11, 19}, .rot = {0, 0, 0}, .size = {12, 13}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1 },
    { .pos = {22, 2, 19}, .rot = {0, 90, 90}, .size = {12, 11}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.4f, 0.4f, 0.4f}, .sector_id = 1 },
    { .pos = {25, -7, 19}, .rot = {0, 90, 90}, .size = {12, 8}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {0.4f, 0.4f, 0.4f}, .sector_id = 1 },
    { .pos = {20, -10, 19}, .rot = {0, 90, 59}, .size = {12, 6}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.4f, 0.4f, 0.4f}, .sector_id = 1 },
    { .pos = {22, -10, 31}, .rot = {0, 0, 0}, .size = {12, 12}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1 },
    { .pos = {22, 1, 19}, .rot = {0, 90, 0}, .size = {12, 1}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 1 },
    { .pos = {22, 1, 19}, .rot = {0, 90, 0}, .size = {12, 1}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.7f, 1.0f}, .sector_id = 1 },
    { .pos = {34, 6, -33}, .rot = {330, 0, 0}, .size = {6, 6}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1 },
    { .pos = {27, 6, -33}, .rot = {330, 0, 0}, .size = {6, 6}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1 },
    { .pos = {20, 6, -33}, .rot = {330, 0, 0}, .size = {6, 6}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1 },
};

static level_quad_t level1_sector2_quads[] = {
    { .pos = {12, -6, 50}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_idx = 1, .is_solid = false, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 2 },
    { .pos = {9, -6, 50}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_idx = 2, .is_solid = false, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 2 },
    { .pos = {6, -6, 50}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_idx = 3, .is_solid = false, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 2 },
    { .pos = {3, -6, 50}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_idx = -1, .is_solid = false, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 2 },
    { .pos = {15, -6, 50}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_idx = 0, .is_solid = false, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 2 },
};

static level_sector_data_t level1_sectors[] = {
    { .id = 0, .light = {1.0f, 1.0f, 1.0f}, .quads = level1_sector0_quads, .quad_count = sizeof(level1_sector0_quads) / sizeof(level1_sector0_quads[0]) },
    { .id = 1, .light = {0.6f, 0.6f, 0.6f}, .quads = level1_sector1_quads, .quad_count = sizeof(level1_sector1_quads) / sizeof(level1_sector1_quads[0]) },
    { .id = 2, .light = {1.0f, 1.0f, 1.0f}, .quads = level1_sector2_quads, .quad_count = sizeof(level1_sector2_quads) / sizeof(level1_sector2_quads[0]) },
};

static inline level_data_t load_1(void)
{
    return (level_data_t){
        .name = "Level 1",
        .path = "level1.h",
        .sectors = level1_sectors,
        .sector_count = sizeof(level1_sectors) / sizeof(level1_sectors[0]),
        .cam = { .pos = {15.530f, -3.066f, 12.320f}, .yaw = -6751.831f, .pitch = -7.376f }
    };
}

#endif
