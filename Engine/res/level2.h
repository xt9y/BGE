#ifndef LEVEL2_H
#define LEVEL2_H

#include "level.h"

static level_quad_t level2_sector0_quads[] = {
    { .pos = {-9, -1, 19}, .rot = {90, 0, 0}, .size = {34, 23}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {0.6f, 0.6f, 0.6f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-4, -1, 2}, .rot = {0, 90, 0}, .size = {14, 10}, .tex_id = 1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 0.8f, 0.7f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-4, -1, 2}, .rot = {0, 10, 0}, .size = {12, 10}, .tex_id = 1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-6, 0, 23}, .rot = {0, 0, 0}, .size = {8, 6}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {0.5f, 1.0f, 1.0f}, .portal_id = 1, .sector_id = 0 },
    { .pos = {5, 0, -6}, .rot = {0, 0, 0}, .size = {8, 6}, .tex_id = 3, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = true, .color = {0.5f, 1.0f, 1.0f}, .portal_id = 1, .sector_id = 0 },
    { .pos = {0, 2, 15}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_id = 2, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {5, 2, 0}, .rot = {0, 0, 0}, .size = {2, 2}, .tex_id = 2, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {-9, -1, 19}, .rot = {0, 0, 0}, .size = {5, 13}, .tex_id = 0, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
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
        .cam = { .pos = {10.538f, 10.429f, 20.034f}, .yaw = -3028.684f, .pitch = -28.979f }
    };
}

#endif
