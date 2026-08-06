#ifndef LEVEL5_H
#define LEVEL5_H

#include "level.h"

static inline level_data_t load_5(void)
{
    return (level_data_t){
        .name = "Level 5",
        .path = "level5.h",
        .sectors = NULL,
        .sector_count = 0,
        .cam = { .pos = {0.0f, 0.0f, 0.0f}, .yaw = 0.0f, .pitch = 0.0f }
    };
}

#endif
