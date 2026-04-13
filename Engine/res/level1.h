#ifndef LEVEL1_H
#define LEVEL1_H

#include "level.h"

static level_quad_t level1_sector0_quads[] = {
    { .pos = {4.600f, -0.800f, -5.600f}, .rot = {0.000f, 0.000f, 0.000f}, .size = {16.400f, 10.600f}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.000f, 1.000f, 1.000f} },
};

static level_quad_t level1_sector1_quads[] = {
    { .pos = {5.100f, -9.800f, 3.000f}, .rot = {90.000f, 0.000f, 0.000f}, .size = {17.500f, 18.700f}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {1.000f, 1.000f, 1.000f} },
    { .pos = {-3.200f, -0.600f, 1.900f}, .rot = {0.000f, -90.000f, 0.000f}, .size = {16.000f, 13.400f}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.500f, 0.000f, 1.000f} },
};

static level_sector_data_t level1_sectors[] = {
    { .id = 0, .light = {0.700f, 0.000f, 1.000f}, .quads = level1_sector0_quads, .quad_count = sizeof(level1_sector0_quads) / sizeof(level1_sector0_quads[0]) },
    { .id = 1, .light = {1.000f, 1.000f, 1.000f}, .quads = level1_sector1_quads, .quad_count = sizeof(level1_sector1_quads) / sizeof(level1_sector1_quads[0]) },
};

static inline level_data_t load_1(void)
{
    return (level_data_t){
        .name = "Level 1",
        .path = "level1.h",
        .sectors = level1_sectors,
        .sector_count = sizeof(level1_sectors) / sizeof(level1_sectors[0]),
        .cam_pos = {19.716f, 9.726f, 19.338f},
        .cam_yaw = 237.300f,
        .cam_pitch = -16.200f
    };
}

#endif
