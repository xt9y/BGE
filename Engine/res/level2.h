#ifndef LEVEL2_H
#define LEVEL2_H

#include "level.h"

static level_quad_t level2_sector0_quads[] = {
    { .pos = {-9, 12, 19}, .rot = {90, 0, 0}, .size = {34, 22}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.4f, 0.4f, 0.4f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-4, -1, 2}, .rot = {0, 303, 0}, .size = {8, 13}, .tex_id = 1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {4, 0, 19}, .rot = {0, 0, 0}, .size = {16, 9}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {0.5f, 1.0f, 1.0f}, .portal_id = 1, .sector_id = 0 },
    { .pos = {4, -1, -3}, .rot = {0, 0, 0}, .size = {16, 9}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {0.5f, 1.0f, 1.0f}, .portal_id = 1, .sector_id = 0 },
    { .pos = {4, 8, -3}, .rot = {0, 0, 0}, .size = {16, 4}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {20, -1, -3}, .rot = {0, 0, 0}, .size = {5, 13}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-9, -1, 19}, .rot = {0, 0, 0}, .size = {13, 13}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {4, -1, 19}, .rot = {0, 0, 0}, .size = {16, 1}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-1, -1, -3}, .rot = {0, 0, 0}, .size = {5, 13}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {20, -1, 19}, .rot = {0, 0, 0}, .size = {5, 13}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {4, 9, 19}, .rot = {0, 0, 0}, .size = {16, 3}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-4, -1, 2}, .rot = {0, 104, 0}, .size = {18, 13}, .tex_id = 1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 0.8f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {19, 30, 22}, .rot = {0, 90, 0}, .size = {14, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.0f, 0.4f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {35, 30, 22}, .rot = {0, 90, 0}, .size = {6, 11}, .tex_id = 1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 0.0f, 0.0f}, .portal_id = 2, .sector_id = 0 },
    { .pos = {31, 27, -11}, .rot = {0, 90, 0}, .size = {6, 11}, .tex_id = 1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 4, .sector_id = 0 },
    { .pos = {25, -1, -2}, .rot = {0, 90, 0}, .size = {6, 11}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 4, .sector_id = 0 },
    { .pos = {25, -1, 5}, .rot = {0, 90, 0}, .size = {6, 11}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 3, .sector_id = 0 },
    { .pos = {25, -1, 12}, .rot = {0, 90, 0}, .size = {6, 11}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 2, .sector_id = 0 },
    { .pos = {31, 38, -19}, .rot = {0, 90, 270}, .size = {14, 16}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 0.0f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {19, 30, 36}, .rot = {0, 0, 0}, .size = {16, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.0f, 0.4f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {33, 27, 8}, .rot = {0, 90, 0}, .size = {6, 11}, .tex_id = 1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 3, .sector_id = 0 },
    { .pos = {15, 27, -19}, .rot = {0, 0, 0}, .size = {16, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.4f, 0.0f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {35, 30, 28}, .rot = {0, 90, 0}, .size = {8, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.0f, 0.4f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {35, 41, 22}, .rot = {0, 90, 270}, .size = {14, 16}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {33, 27, 4}, .rot = {0, 90, 270}, .size = {14, 16}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 1.0f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {31, 27, -19}, .rot = {0, 90, 0}, .size = {8, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.4f, 0.0f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {17, 27, 18}, .rot = {0, 0, 0}, .size = {16, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.4f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {19, 30, 22}, .rot = {0, 0, 0}, .size = {16, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.0f, 0.4f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {15, 27, -19}, .rot = {0, 90, 0}, .size = {14, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.4f, 0.0f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {35, 30, 22}, .rot = {0, 90, 270}, .size = {14, 16}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {17, 27, 4}, .rot = {0, 0, 0}, .size = {16, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.4f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {17, 27, 4}, .rot = {0, 90, 0}, .size = {14, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.4f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {33, 38, 4}, .rot = {0, 90, 270}, .size = {14, 16}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 1.0f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {31, 27, -19}, .rot = {0, 90, 270}, .size = {14, 16}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 0.0f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {15, 27, -5}, .rot = {0, 0, 0}, .size = {16, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.4f, 0.0f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {33, 27, 4}, .rot = {0, 90, 0}, .size = {4, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.4f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-9, -1, 19}, .rot = {90, 0, 0}, .size = {34, 23}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.6f, 0.6f, 0.6f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {25, -1, 18}, .rot = {0, 90, 0}, .size = {1, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.0f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {25, 10, -3}, .rot = {0, 90, 0}, .size = {22, 2}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.0f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {25, -1, 4}, .rot = {0, 90, 0}, .size = {1, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.0f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {25, -1, 11}, .rot = {0, 90, 0}, .size = {1, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.0f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {25, -1, -3}, .rot = {0, 90, 0}, .size = {1, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.0f, 0.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-9, -1, 19}, .rot = {90, 0, 0}, .size = {34, 22}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.6f, 0.6f, 0.6f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {33, 27, 14}, .rot = {0, 90, 0}, .size = {4, 11}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.0f, 0.4f, 0.0f}, .portal_id = 0, .sector_id = 0 },
};

static level_sector_data_t level2_sectors[] = {
    { .id = 0, .light = {1.0f, 1.0f, 1.0f}, .quads = level2_sector0_quads, .quad_count = sizeof(level2_sector0_quads) / sizeof(level2_sector0_quads[0]) },
};

static inline level_data_t load_2(void)
{
    return (level_data_t){
        .name = "Level 2",
        .path = "level2.h",
        .sectors = level2_sectors,
        .sector_count = sizeof(level2_sectors) / sizeof(level2_sectors[0]),
        .cam = { .pos = {7.890f, 3.493f, 7.338f}, .yaw = 78.697f, .pitch = 2.392f }
    };
}

#endif
