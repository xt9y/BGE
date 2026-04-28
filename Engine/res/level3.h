#ifndef LEVEL3_H
#define LEVEL3_H

#include "level.h"

static level_quad_t level3_sector0_quads[] = {
    { .pos = {-7, -4, 16}, .rot = {90, 0, 0}, .size = {22, 11}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {15, 9, 18}, .rot = {90, 0, 0}, .size = {23, 26}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.6f, 0.6f, 0.6f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {15, -1, 9}, .rot = {0, 90, 239}, .size = {7, 7}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-7, -1, 0}, .rot = {0, 90, 0}, .size = {9, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-31, -1, 0}, .rot = {0, 0, 0}, .size = {24, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-31, -1, 0}, .rot = {0, 90, 0}, .size = {7, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-31, -1, 7}, .rot = {0, 90, 0}, .size = {12, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 2, .sector_id = 0 },
    { .pos = {-19, -5, 35}, .rot = {0, 111, 0}, .size = {11, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-19, -1, 25}, .rot = {0, 0, 0}, .size = {12, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-7, -1, 16}, .rot = {0, 90, 0}, .size = {9, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-7, -4, 9}, .rot = {0, 0, 0}, .size = {5, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {10, -4, 5}, .rot = {0, 90, 0}, .size = {4, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {10, -4, 9}, .rot = {0, 0, 0}, .size = {5, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {38, -1, -8}, .rot = {0, 90, 0}, .size = {26, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {25, -1, -1}, .rot = {0, 0, 0}, .size = {3, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {25, -1, -8}, .rot = {0, 90, 0}, .size = {7, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {15, -1, -8}, .rot = {0, 0, 0}, .size = {3, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {15, -1, -8}, .rot = {0, 90, 0}, .size = {17, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {15, -1, 16}, .rot = {0, 90, 0}, .size = {2, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {35, -1, -8}, .rot = {0, 0, 0}, .size = {3, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {15, 6, 9}, .rot = {0, 90, 0}, .size = {7, 3}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-2, -4, 5}, .rot = {0, 90, 0}, .size = {4, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-7, 6, 9}, .rot = {0, 90, 0}, .size = {7, 3}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-35, 9, 45}, .rot = {90, 0, 0}, .size = {16, 20}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {15, -1, 18}, .rot = {90, 0, 0}, .size = {23, 26}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {18, -1, -8}, .rot = {0, 0, 0}, .size = {7, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 1, .sector_id = 0 },
    { .pos = {28, -1, -8}, .rot = {0, 0, 0}, .size = {7, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 1, .sector_id = 0 },
    { .pos = {-2, -4, 5}, .rot = {0, 0, 0}, .size = {12, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 2, .sector_id = 0 },
    { .pos = {25, -1, -8}, .rot = {0, 0, 0}, .size = {3, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {28, -1, -8}, .rot = {0, 90, 0}, .size = {7, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {15, -1, 18}, .rot = {0, 0, 0}, .size = {23, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-7, -4, 16}, .rot = {0, 0, 0}, .size = {22, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-31, 9, 25}, .rot = {90, 0, 0}, .size = {24, 25}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.6f, 0.6f, 0.6f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-7, 6, 16}, .rot = {90, 0, 0}, .size = {22, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-6, 13, 41}, .rot = {0, 0, 0}, .size = {6, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 3, .sector_id = 0 },
    { .pos = {-31, -1, 19}, .rot = {0, 90, 0}, .size = {6, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-35, -5, 36}, .rot = {0, 90, 0}, .size = {9, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-19, -5, 25}, .rot = {0, 90, 0}, .size = {11, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-31, -1, 25}, .rot = {90, 0, 0}, .size = {24, 25}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-25, -5, 31}, .rot = {0, 153, 0}, .size = {12, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-31, -1, 25}, .rot = {0, 0, 0}, .size = {6, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-35, -5, 45}, .rot = {0, 0, 0}, .size = {5, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-35, -5, 45}, .rot = {90, 0, 0}, .size = {16, 20}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-25, -5, 25}, .rot = {0, 90, 0}, .size = {6, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-30, -5, 45}, .rot = {0, 0, 0}, .size = {6, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 3, .sector_id = 0 },
    { .pos = {-24, -5, 45}, .rot = {0, 0, 0}, .size = {2, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-8, 13, 49}, .rot = {90, 0, 0}, .size = {28, 19}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-8, 23, 49}, .rot = {90, 0, 0}, .size = {28, 19}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-5, 17, 38}, .rot = {0, 286, 0}, .size = {2, 2}, .tex_id = 2, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-7, -1, 9}, .rot = {0, 90, 121}, .size = {7, 7}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-35, 5, 45}, .rot = {0, 228, 90}, .size = {7, 15}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-25, -1, 25}, .rot = {0, 49, 90}, .size = {7, 6}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {22, 0, -1}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_id = 2, .is_solid = true, .is_invisible = false, .is_billboard = true, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {1, -3, 8}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_id = 2, .is_solid = true, .is_invisible = false, .is_billboard = true, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
};

static level_quad_t level3_sector1_quads[] = {
    { .pos = {0, 13, 41}, .rot = {0, 47, 0}, .size = {11, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {20, 13, 37}, .rot = {0, 90, 0}, .size = {12, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {13, 13, 30}, .rot = {0, 45, 0}, .size = {11, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {8, 13, 49}, .rot = {0, 0, 0}, .size = {6, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 4, .sector_id = 1 },
    { .pos = {-6, 13, 41}, .rot = {0, 286, 0}, .size = {12, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {-2, 13, 30}, .rot = {0, 0, 0}, .size = {6, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 4, .sector_id = 1 },
    { .pos = {-4, 13, 30}, .rot = {0, 0, 0}, .size = {2, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {4, 13, 30}, .rot = {0, 90, 0}, .size = {2, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {14, 13, 49}, .rot = {0, 0, 0}, .size = {6, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {7, 13, 49}, .rot = {0, 0, 0}, .size = {1, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {5, 13, 30}, .rot = {0, 90, 0}, .size = {2, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {4, 13, 30}, .rot = {0, 0, 0}, .size = {1, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {11, 13, 30}, .rot = {0, 0, 0}, .size = {2, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {5, 13, 30}, .rot = {0, 0, 0}, .size = {6, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {4, 13, 32}, .rot = {0, 0, 0}, .size = {1, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
};

static level_sector_data_t level3_sectors[] = {
    { .id = 0, .light = {0.8f, 0.8f, 0.8f}, .quads = level3_sector0_quads, .quad_count = sizeof(level3_sector0_quads) / sizeof(level3_sector0_quads[0]) },
    { .id = 1, .light = {0.3f, 0.0f, 0.2f}, .quads = level3_sector1_quads, .quad_count = sizeof(level3_sector1_quads) / sizeof(level3_sector1_quads[0]) },
};

static inline level_data_t load_3(void)
{
    return (level_data_t){
        .name = "Level 3",
        .path = "level3.h",
        .sectors = level3_sectors,
        .sector_count = sizeof(level3_sectors) / sizeof(level3_sectors[0]),
        .cam = { .pos = {4.773f, 17.500f, 35.287f}, .yaw = 146.621f, .pitch = -6.975f }
    };
}

#endif
