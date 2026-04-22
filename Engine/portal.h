#ifndef PORTAL_H
#define PORTAL_H

#include "level.h"

typedef struct {
    i32 portal_id;
    const level_quad_t* src;
    const level_quad_t* dst;
} portal_link_t;

bool portal_find_link(const level_data_t* level, const level_quad_t* src, portal_link_t* out_link);
bool portal_build_camera(const level_quad_t* src, const level_quad_t* dst, const camera_t* in_cam, camera_t* out_cam);

#endif // PORTAL_H
