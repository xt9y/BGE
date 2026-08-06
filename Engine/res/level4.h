#ifndef LEVEL4_H
#define LEVEL4_H

#include "level.h"

static level_quad_t level4_sector0_quads[] = {
    { .pos = {14, 4, 3}, .rot = {90, 0, 0}, .size = {18, 17}, .tex_id = 9, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {14, 4, -14}, .rot = {0, 0, 0}, .size = {18, 8}, .tex_id = 1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
    { .pos = {14, 4, 3}, .rot = {0, 0, 0}, .size = {18, 8}, .tex_id = 1, .is_solid = true, .is_invisible = false, .is_billboard = false, .portal_side_flip = false, .color = {1.0f, 1.0f, 1.0f}, .portal_id = 0, .sector_id = 0 },
};

static level_sector_data_t level4_sectors[] = {
    { .id = 0, .light = {1.0f, 1.0f, 1.0f}, .quads = level4_sector0_quads, .quad_count = sizeof(level4_sector0_quads) / sizeof(level4_sector0_quads[0]) },
};

static inline level_data_t load_4(void)
{
    return (level_data_t){
        .name = "Level 4",
        .path = "level4.h",
        .sectors = level4_sectors,
        .sector_count = sizeof(level4_sectors) / sizeof(level4_sectors[0]),
        .cam = { .pos = {34.257f, 22.131f, -0.222f}, .yaw = -122.178f, .pitch = -61.243f }
    };
}

#endif
