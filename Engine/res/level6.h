#ifndef LEVEL6_H
#define LEVEL6_H

#include "level.h"

static inline level_data_t load_6(void)
{
    return (level_data_t){
        .name = "Level 6",
        .path = "level6.h",
        .sectors = NULL,
        .sector_count = 0,
        .cam = { .pos = {0.0f, 0.0f, 0.0f}, .yaw = 0.0f, .pitch = 0.0f }
    };
}

#endif
