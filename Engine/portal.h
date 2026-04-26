#ifndef PORTAL_H
#define PORTAL_H

#include "level.h"
#include "cam.h"

#define MAX_PORTAL_DEPTH 2
#define PORTAL_SUCTION_DIST 1.2f

typedef struct {
    i32 portal_id;
    const level_quad_t* src;
    const level_quad_t* dst;
} portal_link_t;

typedef struct {
    vec3s right;
    vec3s up;
    vec3s normal;
} portal_basis_t;

bool portal_try_teleport(const level_data_t* level, vec3s prev_pos, camera_t* cam);
bool portal_find_link(const level_data_t* level, const level_quad_t* src, portal_link_t* out_link);
bool portal_build_camera(const level_quad_t* src, const level_quad_t* dst, const camera_t* in_cam, camera_t* out_cam);

#endif // PORTAL_H
