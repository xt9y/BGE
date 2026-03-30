#ifndef LEVEL3_H
#define LEVEL3_H

#include "level.h"

// Sector 0: Starting Room (Octagonal spawn area)
static const level_quad_t level3_sector0_quads[] = {
    { .pos = {  0.0f, 0.0f, -2.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {4.0f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {  2.5f, 0.0f, -1.5f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {1.4f, 3.0f}, .tex_idx = -1, .is_solid = false, .is_invisible = true,  .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {  3.0f, 0.0f,  0.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.0f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {  2.5f, 0.0f,  1.5f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {1.4f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = {  0.0f, 0.0f,  2.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {4.0f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = { -2.5f, 0.0f,  1.5f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {1.4f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = { -3.0f, 0.0f,  0.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.0f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
    { .pos = { -2.5f, 0.0f, -1.5f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {1.4f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {1.0f, 1.0f, 1.0f} },
};

// Sector 1: Corridor to Main Hall (Narrow passage)
static const level_quad_t level3_sector1_quads[] = {
    { .pos = {  2.0f, 0.0f, -4.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {4.0f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {1.0f, 0.5f, 0.5f} },
    { .pos = {  3.0f, 0.0f, -6.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.0f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {1.0f, 0.5f, 0.5f} },
    { .pos = {  4.0f, 0.0f, -4.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {4.0f, 3.0f}, .tex_idx = -1, .is_solid = false, .is_invisible = true,  .color = {1.0f, 0.5f, 0.5f} },
    { .pos = {  3.5f, 0.0f, -1.5f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.1f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {1.0f, 0.5f, 0.5f} },
    { .pos = {  2.5f, 0.0f, -1.5f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {1.4f, 3.0f}, .tex_idx = -1, .is_solid = false, .is_invisible = true,  .color = {1.0f, 0.5f, 0.5f} },
};

// Sector 2: Main Hall (Large rectangular room)
static const level_quad_t level3_sector2_quads[] = {
    { .pos = {  4.0f, 0.0f, -8.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {4.0f, 4.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 1.0f, 0.5f} },
    { .pos = {  5.0f, 0.0f,-11.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.8f, 4.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 1.0f, 0.5f} },
    { .pos = {  8.0f, 0.0f,-12.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {4.0f, 4.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 1.0f, 0.5f} },
    { .pos = { 11.0f, 0.0f,-11.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.8f, 4.0f}, .tex_idx = -1, .is_solid = false, .is_invisible = true,  .color = {0.5f, 1.0f, 0.5f} },
    { .pos = { 12.0f, 0.0f, -8.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {4.0f, 4.0f}, .tex_idx = -1, .is_solid = false, .is_invisible = true,  .color = {0.5f, 1.0f, 0.5f} },
    { .pos = { 11.0f, 0.0f, -5.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.8f, 4.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 1.0f, 0.5f} },
    { .pos = {  8.0f, 0.0f, -4.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {4.0f, 4.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 1.0f, 0.5f} },
    { .pos = {  5.0f, 0.0f, -3.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.8f, 4.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 1.0f, 0.5f} },
    { .pos = {  4.0f, 0.0f, -4.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.0f, 4.0f}, .tex_idx = -1, .is_solid = false, .is_invisible = true,  .color = {0.5f, 1.0f, 0.5f} },
};

// Sector 3: Side Alcove (Small dark room)
static const level_quad_t level3_sector3_quads[] = {
    { .pos = { 10.0f, 0.0f,-13.5f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {3.0f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 0.5f, 1.0f} },
    { .pos = { 12.0f, 0.0f,-15.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {4.0f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 0.5f, 1.0f} },
    { .pos = { 14.0f, 0.0f,-13.5f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {3.0f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 0.5f, 1.0f} },
    { .pos = { 13.0f, 0.0f,-11.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.8f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 0.5f, 1.0f} },
    { .pos = { 11.0f, 0.0f,-11.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {1.4f, 3.0f}, .tex_idx = -1, .is_solid = false, .is_invisible = true,  .color = {0.5f, 0.5f, 1.0f} },
};

// Sector 4: Exit Corridor (Angled passage)
static const level_quad_t level3_sector4_quads[] = {
    { .pos = { 13.0f, 0.0f, -8.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.8f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {1.0f, 1.0f, 0.5f} },
    { .pos = { 14.0f, 0.0f, -4.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.0f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {1.0f, 1.0f, 0.5f} },
    { .pos = { 16.0f, 0.0f, -6.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {4.0f, 3.0f}, .tex_idx = -1, .is_solid = false, .is_invisible = true,  .color = {1.0f, 1.0f, 0.5f} },
    { .pos = { 14.0f, 0.0f, -9.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {4.5f, 3.0f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {1.0f, 1.0f, 0.5f} },
    { .pos = { 12.0f, 0.0f, -8.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.0f, 3.0f}, .tex_idx = -1, .is_solid = false, .is_invisible = true,  .color = {1.0f, 1.0f, 0.5f} },
};

// Sector 5: Final Room (Hexagonal end room)
static const level_quad_t level3_sector5_quads[] = {
    { .pos = { 17.0f, 0.0f, -3.5f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.2f, 3.5f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 1.0f, 1.0f} },
    { .pos = { 19.0f, 0.0f, -3.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.0f, 3.5f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 1.0f, 1.0f} },
    { .pos = { 20.5f, 0.0f, -4.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.2f, 3.5f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 1.0f, 1.0f} },
    { .pos = { 21.0f, 0.0f, -6.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.0f, 3.5f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 1.0f, 1.0f} },
    { .pos = { 20.5f, 0.0f, -8.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.2f, 3.5f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 1.0f, 1.0f} },
    { .pos = { 19.0f, 0.0f, -9.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.0f, 3.5f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 1.0f, 1.0f} },
    { .pos = { 17.0f, 0.0f, -8.5f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {2.2f, 3.5f}, .tex_idx = -1, .is_solid = true,  .is_invisible = false, .color = {0.5f, 1.0f, 1.0f} },
    { .pos = { 16.0f, 0.0f, -6.0f }, .rot = {0.0f,   0.0f, 0.0f}, .size = {4.0f, 3.5f}, .tex_idx = -1, .is_solid = false, .is_invisible = true,  .color = {0.5f, 1.0f, 1.0f} },
};

static const level_sector_data_t level3_sectors[] = {
    { .id = 0, .light_intensity = 0.7f, .floor_height = 0.0f, .ceil_height = 3.0f, .quads = level3_sector0_quads, .quad_count = sizeof(level3_sector0_quads) / sizeof(level3_sector0_quads[0]) },
    { .id = 1, .light_intensity = 0.4f, .floor_height = 0.0f, .ceil_height = 2.5f, .quads = level3_sector1_quads, .quad_count = sizeof(level3_sector1_quads) / sizeof(level3_sector1_quads[0]) },
    { .id = 2, .light_intensity = 0.5f, .floor_height = 0.0f, .ceil_height = 4.0f, .quads = level3_sector2_quads, .quad_count = sizeof(level3_sector2_quads) / sizeof(level3_sector2_quads[0]) },
    { .id = 3, .light_intensity = 0.3f, .floor_height = 0.5f, .ceil_height = 3.5f, .quads = level3_sector3_quads, .quad_count = sizeof(level3_sector3_quads) / sizeof(level3_sector3_quads[0]) },
    { .id = 4, .light_intensity = 0.7f, .floor_height = 0.0f, .ceil_height = 3.0f, .quads = level3_sector4_quads, .quad_count = sizeof(level3_sector4_quads) / sizeof(level3_sector4_quads[0]) },
    { .id = 5, .light_intensity = 0.9f, .floor_height = 0.0f, .ceil_height = 3.5f, .quads = level3_sector5_quads, .quad_count = sizeof(level3_sector5_quads) / sizeof(level3_sector5_quads[0]) },
};

static inline level_t load_3(void)
{
    const level_data_t data = {
        .name = "Level 3",
        .path = "level3.h",
        .sectors = level3_sectors,
        .sector_count = sizeof(level3_sectors) / sizeof(level3_sectors[0])
    };
    return level_load(&data);
}

#endif
