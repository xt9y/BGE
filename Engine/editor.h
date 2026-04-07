#ifndef EDITOR_H
#define EDITOR_H

#include "level.h"
#include "cam.h"

typedef struct {
    level_data_t *level;
} editor_t;

void editor_render(const level_data_t *level);
void editor_render_look_at_info(const level_data_t *level, const camera_t *cam);

#endif

