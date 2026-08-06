#ifndef LEVEL7_H
#define LEVEL7_H

#include "level.h"

static inline level_data_t load_7(void)
{
    return (level_data_t){
        .name = "Level 7",
        .path = "level7.h",
        .sectors = NULL,
        .sector_count = 0,
        .cam = { .pos = {0.0f, 0.0f, 0.0f}, .yaw = 0.0f, .pitch = 0.0f }
    };
}

#endif
