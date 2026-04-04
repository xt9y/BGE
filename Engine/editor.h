#ifndef EDITOR_H
#define EDITOR_H

#include "level.h"

typedef struct {
    level_data_t *level;
} editor_t;

void editor_render(const level_data_t *level);

#endif

