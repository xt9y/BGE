#ifndef LEVEL3_H
#define LEVEL3_H

#include "level.h"

static level_quad_t level3_sector0_quads[] = {
    { .pos = {-7, 6, 16}, .rot = {90, 0, 0}, .size = {22, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-7, -4, 16}, .rot = {90, 0, 0}, .size = {22, 11}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {15, -1, 16}, .rot = {90, 0, 0}, .size = {23, 24}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {15, 9, 16}, .rot = {90, 0, 0}, .size = {23, 24}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.6f, 0.6f, 0.6f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-31, 9, 25}, .rot = {90, 0, 0}, .size = {24, 25}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.6f, 0.6f, 0.6f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-31, -1, 25}, .rot = {90, 0, 0}, .size = {24, 25}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {15, -1, 9}, .rot = {0, 90, 239}, .size = {7, 7}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-7, -1, 9}, .rot = {0, 90, 121}, .size = {7, 7}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 0 },
};

static level_quad_t level3_sector1_quads[] = {
    { .pos = {10, -4, 9}, .rot = {0, 0, 0}, .size = {5, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {-31, -1, 0}, .rot = {0, 0, 0}, .size = {24, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {-2, -4, 5}, .rot = {0, 90, 0}, .size = {4, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {38, -1, -8}, .rot = {0, 90, 0}, .size = {24, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {28, -1, -8}, .rot = {0, 0, 0}, .size = {7, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 1, .sector_id = 1 },
    { .pos = {15, -1, -8}, .rot = {0, 90, 0}, .size = {17, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {-7, -1, 16}, .rot = {0, 90, 0}, .size = {9, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {-31, -1, 25}, .rot = {0, 0, 0}, .size = {24, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {15, -1, 16}, .rot = {0, 0, 0}, .size = {23, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {-31, -1, 7}, .rot = {0, 90, 0}, .size = {12, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 2, .sector_id = 1 },
    { .pos = {-31, -1, 19}, .rot = {0, 90, 0}, .size = {6, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {-31, -1, 0}, .rot = {0, 90, 0}, .size = {7, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {-7, -1, 0}, .rot = {0, 90, 0}, .size = {9, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {-7, -1, 0}, .rot = {0, 90, 0}, .size = {9, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {-7, -4, 16}, .rot = {0, 0, 0}, .size = {22, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {-7, -4, 9}, .rot = {0, 0, 0}, .size = {5, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {18, -1, -8}, .rot = {0, 0, 0}, .size = {7, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 1, .sector_id = 1 },
    { .pos = {-2, -4, 5}, .rot = {0, 0, 0}, .size = {12, 10}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 2, .sector_id = 1 },
    { .pos = {-7, 6, 9}, .rot = {0, 90, 0}, .size = {7, 3}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {15, 6, 9}, .rot = {0, 90, 0}, .size = {7, 3}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {25, -1, -8}, .rot = {0, 90, 0}, .size = {7, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {35, -1, -8}, .rot = {0, 0, 0}, .size = {3, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {15, -1, -8}, .rot = {0, 0, 0}, .size = {3, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {25, -1, -1}, .rot = {0, 0, 0}, .size = {3, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {28, -1, -8}, .rot = {0, 90, 0}, .size = {7, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {25, -1, -8}, .rot = {0, 0, 0}, .size = {3, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 1 },
    { .pos = {10, -4, 5}, .rot = {0, 90, 0}, .size = {4, 10}, .tex_id = -1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {0.7f, 0.7f, 0.7f}, .portal_id = 0, .sector_id = 1 },
};

static level_quad_t level3_sector2_quads[] = {
    { .pos = {20, 3, -5}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_id = 2, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 2 },
};

static level_sector_data_t level3_sectors[] = {
    { .id = 0, .light = {0.8f, 0.8f, 0.8f}, .quads = level3_sector0_quads, .quad_count = sizeof(level3_sector0_quads) / sizeof(level3_sector0_quads[0]) },
    { .id = 1, .light = {0.7f, 0.0f, 1.0f}, .quads = level3_sector1_quads, .quad_count = sizeof(level3_sector1_quads) / sizeof(level3_sector1_quads[0]) },
    { .id = 2, .light = {1.0f, 1.0f, 1.0f}, .quads = level3_sector2_quads, .quad_count = sizeof(level3_sector2_quads) / sizeof(level3_sector2_quads[0]) },
};

static inline level_data_t load_3(void)
{
    return (level_data_t){
        .name = "Level 3",
        .path = "level3.h",
        .sectors = level3_sectors,
        .sector_count = sizeof(level3_sectors) / sizeof(level3_sectors[0]),
        .cam = { .pos = {15.923f, 3.500f, 13.158f}, .yaw = -3298.806f, .pitch = 0.937f }
    };
}

#endif
