#ifndef BGE_RENDER_H
#define BGE_RENDER_H

#include "cam.h"
#include "level.h"

#define MAX_DEBUG_TRACE_POINTS 8

bool render_init(void);
void render_shutdown(void);
bool render_main(i32 rw, i32 rh);
void render_debug_trace_add(const vec3s* points, i32 point_count, vec3s normal, vec3s color);
void apply_level_camera(camera_t* cam, const level_data_t* level);

#endif
