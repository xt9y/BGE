#ifndef LEVEL2_H
#define LEVEL2_H

#include "level.h"

static level_quad_t level2_sector0_quads[] = {
    { .pos = {0.000f, 0.000f, 0.000f}, .rot = {0.000f, 90.000f, 0.000f}, .size = {30.000f, 0.100f}, .tex_idx = 3, .is_solid = true, .is_invisible = true, .color = {1.000f, 0.000f, 0.000f} },
    { .pos = {0.000f, 0.000f, 0.000f}, .rot = {0.000f, 0.000f, 90.000f}, .size = {30.000f, 0.100f}, .tex_idx = 3, .is_solid = true, .is_invisible = true, .color = {0.000f, 1.000f, 0.000f} },
    { .pos = {0.000f, 0.000f, 0.000f}, .rot = {0.000f, 0.000f, 0.000f}, .size = {30.000f, 0.100f}, .tex_idx = 3, .is_solid = true, .is_invisible = true, .color = {0.000f, 0.000f, 1.000f} },
    { .pos = {15.568f, 2.807f, 11.841f}, .rot = {0.000f, -26.000f, 0.000f}, .size = {7.973f, 6.076f}, .tex_idx = 3, .is_solid = true, .is_invisible = false, .color = {1.000f, 1.000f, 1.000f} },
};

static level_quad_t level2_sector1_quads[] = {
    { .pos = {5.700f, -0.700f, 0.500f}, .rot = {0.000f, 10.000f, 0.000f}, .size = {12.400f, 10.100f}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {1.000f, 0.300f, 0.300f} },
    { .pos = {3.100f, -7.700f, 4.900f}, .rot = {90.000f, 0.000f, 0.000f}, .size = {20.500f, 14.700f}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {0.600f, 0.000f, 0.000f} },
    { .pos = {-0.300f, -0.500f, 4.500f}, .rot = {0.000f, 90.000f, 0.000f}, .size = {12.600f, 9.900f}, .tex_idx = 1, .is_solid = true, .is_invisible = false, .color = {0.700f, 0.000f, 0.000f} },
};

static level_sector_data_t level2_sectors[] = {
    { .id = 0, .light = {1.000f, 1.000f, 1.000f}, .quads = level2_sector0_quads, .quad_count = sizeof(level2_sector0_quads) / sizeof(level2_sector0_quads[0]) },
    { .id = 1, .light = {1.000f, 1.000f, 1.000f}, .quads = level2_sector1_quads, .quad_count = sizeof(level2_sector1_quads) / sizeof(level2_sector1_quads[0]) },
};

static inline level_data_t load_2(void)
{
    return (level_data_t){
        .name = "Level 2",
        .path = "level2.h",
        .sectors = level2_sectors,
        .sector_count = sizeof(level2_sectors) / sizeof(level2_sectors[0]),
        .cam_pos = {19.336f, 8.410f, 20.049f},
        .cam_yaw = 245.100f,
        .cam_pitch = -14.000f
    };
}

#endif
