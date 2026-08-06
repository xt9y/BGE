#ifndef LEVEL8_H
#define LEVEL8_H

#include "level.h"

static inline level_data_t load_8(void)
{
    return (level_data_t){
        .name = "Level 8",
        .path = "level8.h",
        .sectors = NULL,
        .sector_count = 0,
        .cam = { .pos = {0.0f, 0.0f, 0.0f}, .yaw = 0.0f, .pitch = 0.0f }
    };
}

#endif
