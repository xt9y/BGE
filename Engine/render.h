#ifndef RENDER_H
#define RENDER_H

#include "level.h"
#include "cam.h"

void render_init(void);
void render_shutdown(void);
void render_main(i32 rw, i32 rh);
void shoot_bullet(void);
void apply_level_camera(camera_t *cam, level_data_t *level);
void player_collide_quads(const level_data_t* level, camera_t* cam);

#endif
