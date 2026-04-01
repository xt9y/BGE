#ifndef LEVEL3_H
#define LEVEL3_H

#include "level.h"

static const level_quad_t level3_sector0_quads[] = {
    { .pos = {  10.0f,  0.0f,  1.0f}, .rot = {  0.0f, 20.0f,  0.0f}, .size = {3.0f, 3.0f}, .tex_idx =  1, .is_solid = true, .is_invisible = false, .color = {1.0f, 0.0f, 1.0f} },
    { .pos = {  18.0f, -2.0f, -7.0f}, .rot = {  0.0f, 80.0f,  0.0f}, .size = {4.0f, 13.0f}, .tex_idx = 0, .is_solid = true, .is_invisible = false, .color = {0.6f, 0.0f, 1.0f} },
};

static const level_quad_t level3_sector1_quads[] = {
    { .pos = {  12.0f,  0.0f, -1.0f}, .rot = {  0.0f, 80.0f,  0.0f}, .size = {4.0f, 3.0f}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {  15.0f,  0.0f, -4.0f}, .rot = {  0.0f, 80.0f,  0.0f}, .size = {4.0f, 3.0f}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {1.0f, 0.0f, 0.0f} },
    { .pos = {  21.0f,  0.0f, -10.0f}, .rot = {  0.0f, 80.0f,  0.0f}, .size = {4.0f, 3.0f}, .tex_idx = -1, .is_solid = true, .is_invisible = false, .color = {0.0f, 1.0f, 1.0f} },
};

static const level_sector_data_t level3_sectors[] = {
    { .id = 0, .light_intensity = 0.2f, .quads = level3_sector0_quads, .quad_count = sizeof(level3_sector0_quads) / sizeof(level3_sector0_quads[0]) },
    { .id = 1, .light_intensity = 0.4f, .quads = level3_sector1_quads, .quad_count = sizeof(level3_sector1_quads) / sizeof(level3_sector1_quads[0]) },
};

static inline level_data_t load_3(void)
{
    return (level_data_t){
        .name = "Level 3",
        .path = "level3.h",
        .sectors = level3_sectors,
        .sector_count = sizeof(level3_sectors) / sizeof(level3_sectors[0])
    };
}

#endif
