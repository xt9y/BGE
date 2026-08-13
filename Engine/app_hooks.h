#ifndef BGE_APP_HOOKS_H
#define BGE_APP_HOOKS_H

#include "level.h"
#include "util/types.h"
#include <stdbool.h>

/* Implemented by the application/game layer linked with BGE. */
bool bge_app_register_resources(void);
bool bge_app_init(level_data_t* level);
void bge_app_update(level_data_t* level);
void bge_app_render_scene_overlay(i32 rw, i32 rh);
void bge_app_render_ui(i32 width, i32 height);
void bge_app_shutdown(void);

#endif
