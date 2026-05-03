#ifndef LEVEL3_H
#define LEVEL3_H

#include "level.h"

static level_quad_t level3_sector0_quads[] = {
    { .pos = {-7, -4, 16}, .rot = {90, 0, 0}, .size = {22, 11}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {15, -1, 9}, .rot = {0, 90, 239}, .size = {7, 7}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-31, -1, 7}, .rot = {0, 90, 0}, .size = {12, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 2, .sector_id = 0 },
    { .pos = {15, -1, 18}, .rot = {90, 0, 0}, .size = {23, 26}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {18, -1, -8}, .rot = {0, 0, 0}, .size = {7, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 1, .sector_id = 0 },
    { .pos = {28, -1, -8}, .rot = {0, 0, 0}, .size = {7, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 1, .sector_id = 0 },
    { .pos = {-2, -4, 5}, .rot = {0, 0, 0}, .size = {12, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 2, .sector_id = 0 },
    { .pos = {25, -1, -8}, .rot = {0, 0, 0}, .size = {3, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-31, 9, 25}, .rot = {90, 0, 0}, .size = {24, 25}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.6f, 0.6f, 0.6f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-7, 6, 16}, .rot = {90, 0, 0}, .size = {22, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.9f, 0.9f, 0.9f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-8, 13, 23}, .rot = {0, 0, 0}, .size = {6, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 4, .sector_id = 0 },
    { .pos = {-31, -1, 25}, .rot = {90, 0, 0}, .size = {24, 25}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-8, 27, 41}, .rot = {90, 0, 0}, .size = {21, 18}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-30, -5, 45}, .rot = {0, 0, 0}, .size = {6, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 3, .sector_id = 0 },
    { .pos = {-7, -1, 9}, .rot = {0, 90, 121}, .size = {7, 7}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-35, 5, 45}, .rot = {0, 228, 90}, .size = {7, 15}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.6f, 0.6f, 0.6f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-8, 16, 41}, .rot = {0, 300, 90}, .size = {7, 6}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {15, 9, 18}, .rot = {90, 0, 0}, .size = {23, 26}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.6f, 0.6f, 0.6f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-35, 9, 45}, .rot = {90, 0, 0}, .size = {16, 20}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.6f, 0.6f, 0.6f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-8, 16, 41}, .rot = {0, 0, 0}, .size = {6, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 3, .sector_id = 0 },
    { .pos = {7, 13, 41}, .rot = {0, 0, 0}, .size = {6, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 4, .sector_id = 0 },
    { .pos = {-8, 13, 41}, .rot = {90, 0, 0}, .size = {21, 18}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-35, -5, 45}, .rot = {90, 0, 0}, .size = {16, 20}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-35, 5, 45}, .rot = {90, 0, 0}, .size = {16, 20}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.9f, 0.9f, 0.9f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-25, -1, 25}, .rot = {0, 49, 90}, .size = {7, 6}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {1, 23, 41}, .rot = {90, 0, 0}, .size = {12, 2}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-7, 17, 15}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_id = 8, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
};

static level_quad_t level3_sector1_quads[] = {
    { .pos = {15, -1, -8}, .rot = {0, 90, 0}, .size = {17, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 2 },
    { .pos = {15, 6, 9}, .rot = {0, 90, 0}, .size = {7, 3}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 2 },
    { .pos = {15, -1, 16}, .rot = {0, 90, 0}, .size = {2, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 2 },
    { .pos = {15, -1, 18}, .rot = {0, 0, 0}, .size = {23, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 2 },
    { .pos = {38, -1, -8}, .rot = {0, 90, 0}, .size = {26, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 2 },
    { .pos = {35, -1, -8}, .rot = {0, 0, 0}, .size = {3, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 2 },
    { .pos = {28, -1, -8}, .rot = {0, 90, 0}, .size = {7, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 2 },
    { .pos = {25, -1, -1}, .rot = {0, 0, 0}, .size = {3, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 2 },
    { .pos = {15, -1, -8}, .rot = {0, 0, 0}, .size = {3, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 2 },
    { .pos = {25, -1, -8}, .rot = {0, 90, 0}, .size = {7, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 2 },
};

static level_quad_t level3_sector2_quads[] = {
    { .pos = {-7, -1, 0}, .rot = {0, 90, 0}, .size = {9, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 3 },
    { .pos = {-31, -1, 0}, .rot = {0, 0, 0}, .size = {24, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 3 },
    { .pos = {-31, -1, 0}, .rot = {0, 90, 0}, .size = {7, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 3 },
    { .pos = {-31, -1, 19}, .rot = {0, 90, 0}, .size = {6, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 3 },
    { .pos = {-31, -1, 25}, .rot = {0, 0, 0}, .size = {6, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 3 },
    { .pos = {-19, -1, 25}, .rot = {0, 0, 0}, .size = {12, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 3 },
    { .pos = {-7, -1, 16}, .rot = {0, 90, 0}, .size = {9, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 3 },
    { .pos = {-25, 5, 25}, .rot = {0, 0, 0}, .size = {6, 4}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 3 },
};

static level_quad_t level3_sector3_quads[] = {
    { .pos = {-7, -4, 16}, .rot = {0, 0, 0}, .size = {22, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 4 },
    { .pos = {10, -4, 5}, .rot = {0, 90, 0}, .size = {4, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 4 },
    { .pos = {10, -4, 9}, .rot = {0, 0, 0}, .size = {5, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 4 },
    { .pos = {-7, -4, 9}, .rot = {0, 0, 0}, .size = {5, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 4 },
    { .pos = {-2, -4, 5}, .rot = {0, 90, 0}, .size = {4, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 4 },
    { .pos = {-7, 6, 9}, .rot = {0, 90, 0}, .size = {7, 3}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 4 },
};

static level_quad_t level3_sector4_quads[] = {
    { .pos = {-25, -5, 25}, .rot = {0, 90, 0}, .size = {6, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 5 },
    { .pos = {-19, -5, 25}, .rot = {0, 90, 0}, .size = {11, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 5 },
    { .pos = {-24, -5, 45}, .rot = {0, 299, 0}, .size = {11, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 5 },
    { .pos = {-35, -5, 45}, .rot = {0, 0, 0}, .size = {5, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 5 },
    { .pos = {-35, -5, 36}, .rot = {0, 90, 0}, .size = {9, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 5 },
    { .pos = {-25, -5, 31}, .rot = {0, 153, 0}, .size = {12, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 5 },
};

static level_quad_t level3_sector5_quads[] = {
    { .pos = {-8, 13, 23}, .rot = {0, 90, 0}, .size = {18, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 6 },
    { .pos = {13, 13, 23}, .rot = {0, 90, 0}, .size = {18, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 6 },
    { .pos = {-2, 13, 23}, .rot = {0, 0, 0}, .size = {15, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 6 },
    { .pos = {1, 13, 41}, .rot = {0, 0, 0}, .size = {6, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 6 },
    { .pos = {1, 23, 39}, .rot = {0, 0, 0}, .size = {12, 4}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 6 },
    { .pos = {-8, 26, 41}, .rot = {0, 0, 0}, .size = {6, 1}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 6 },
    { .pos = {-8, 23, 23}, .rot = {0, 0, 0}, .size = {6, 4}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 6 },
    { .pos = {-2, 13, 36}, .rot = {0, 90, 0}, .size = {5, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 6 },
    { .pos = {1, 13, 36}, .rot = {0, 90, 0}, .size = {5, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 6 },
    { .pos = {-2, 13, 36}, .rot = {0, 0, 0}, .size = {3, 14}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 6 },
};

static level_sector_data_t level3_sectors[] = {
    { .id = 0, .light = {1.0f, 1.0f, 1.0f}, .quads = level3_sector0_quads, .quad_count = sizeof(level3_sector0_quads) / sizeof(level3_sector0_quads[0]) },
    { .id = 2, .light = {1.0f, 0.9f, 1.0f}, .quads = level3_sector1_quads, .quad_count = sizeof(level3_sector1_quads) / sizeof(level3_sector1_quads[0]) },
    { .id = 3, .light = {0.8f, 0.9f, 0.9f}, .quads = level3_sector2_quads, .quad_count = sizeof(level3_sector2_quads) / sizeof(level3_sector2_quads[0]) },
    { .id = 4, .light = {1.0f, 0.9f, 0.9f}, .quads = level3_sector3_quads, .quad_count = sizeof(level3_sector3_quads) / sizeof(level3_sector3_quads[0]) },
    { .id = 5, .light = {0.9f, 1.0f, 0.9f}, .quads = level3_sector4_quads, .quad_count = sizeof(level3_sector4_quads) / sizeof(level3_sector4_quads[0]) },
    { .id = 6, .light = {0.8f, 0.9f, 1.0f}, .quads = level3_sector5_quads, .quad_count = sizeof(level3_sector5_quads) / sizeof(level3_sector5_quads[0]) },
};

static inline level_data_t load_3(void)
{
    return (level_data_t){
        .name = "Level 3",
        .path = "level3.h",
        .sectors = level3_sectors,
        .sector_count = sizeof(level3_sectors) / sizeof(level3_sectors[0]),
        .cam = { .pos = {4.939f, 17.500f, 35.106f}, .yaw = -134.284f, .pitch = -2.900f }
    };
}

#endif
