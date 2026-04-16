#ifndef LEVEL3_H
#define LEVEL3_H

#include "level.h"

static level_quad_t level3_sector0_quads[] = {
    { .pos = {-7, 6, 16}, .rot = {90, 0, 0}, .size = {22, 11}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0, .portal_id = 0 },
    { .pos = {-7, -4, 16}, .rot = {90, 0, 0}, .size = {22, 11}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0, .portal_id = 0 },
    { .pos = {15, -1, 16}, .rot = {90, 0, 0}, .size = {24, 24}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0, .portal_id = 0 },
    { .pos = {15, 9, 16}, .rot = {90, 0, 0}, .size = {24, 24}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.6f, 0.6f}, .sector_id = 0, .portal_id = 0 },
    { .pos = {-31, 9, 25}, .rot = {90, 0, 0}, .size = {24, 25}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.6f, 0.6f}, .sector_id = 0, .portal_id = 0 },
    { .pos = {-31, -1, 25}, .rot = {90, 0, 0}, .size = {24, 25}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0, .portal_id = 0 },
    { .pos = {15, -1, 9}, .rot = {0, 90, 239}, .size = {7, 7}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0, .portal_id = 0 },
    { .pos = {-7, -1, 9}, .rot = {0, 90, 121}, .size = {7, 7}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 0, .portal_id = 0 },
};

static level_quad_t level3_sector1_quads[] = {
    { .pos = {10, -4, 9}, .rot = {0, 0, 0}, .size = {5, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {15, -1, -8}, .rot = {0, 0, 0}, .size = {24, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {-26, -1, 0}, .rot = {0, 0, 0}, .size = {19, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {7, -4, 5}, .rot = {0, 53, 0}, .size = {5, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {39, -1, -8}, .rot = {0, 90, 0}, .size = {24, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {32, -1, 2}, .rot = {0, 0, 0}, .size = {7, 10}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 1 },
    { .pos = {15, -1, -8}, .rot = {0, 90, 0}, .size = {17, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {-7, -1, 16}, .rot = {0, 90, 0}, .size = {9, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {-26, -1, 25}, .rot = {0, 0, 0}, .size = {19, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {15, -1, 16}, .rot = {0, 0, 0}, .size = {24, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {-31, -1, 8}, .rot = {0, 90, 0}, .size = {9, 10}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 2 },
    { .pos = {-31, -1, 17}, .rot = {0, 57, 0}, .size = {10, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {-26, -1, 0}, .rot = {0, 122, 0}, .size = {10, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {-7, -1, 0}, .rot = {0, 90, 0}, .size = {9, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {-7, -1, 0}, .rot = {0, 90, 0}, .size = {9, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {-7, -4, 16}, .rot = {0, 0, 0}, .size = {22, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {-2, -4, 9}, .rot = {0, 316, 0}, .size = {6, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {-7, -4, 9}, .rot = {0, 0, 0}, .size = {5, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {29, -1, -8}, .rot = {0, 90, 0}, .size = {7, 10}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 1 },
    { .pos = {2, -4, 5}, .rot = {0, 0, 0}, .size = {5, 10}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 2 },
    { .pos = {-7, 6, 9}, .rot = {0, 90, 0}, .size = {7, 3}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {15, 6, 9}, .rot = {0, 90, 0}, .size = {7, 3}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {29, -1, -1}, .rot = {0, 90, 0}, .size = {3, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
    { .pos = {29, -1, 2}, .rot = {0, 0, 0}, .size = {3, 10}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f}, .sector_id = 1, .portal_id = 0 },
};

static level_sector_data_t level3_sectors[] = {
    { .id = 0, .light = {0.8f, 0.8f, 0.8f}, .quads = level3_sector0_quads, .quad_count = sizeof(level3_sector0_quads) / sizeof(level3_sector0_quads[0]) },
    { .id = 1, .light = {0.7f, 0.0f, 1.0f}, .quads = level3_sector1_quads, .quad_count = sizeof(level3_sector1_quads) / sizeof(level3_sector1_quads[0]) },
};

static inline level_data_t load_3(void)
{
    return (level_data_t){
        .name = "Level 3",
        .path = "level3.h",
        .sectors = level3_sectors,
        .sector_count = sizeof(level3_sectors) / sizeof(level3_sectors[0]),
        .cam = { .pos = {22.012f, 3.500f, 11.365f}, .yaw = -6164.250f, .pitch = 21.300f }
    };
}

#endif
