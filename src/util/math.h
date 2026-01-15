#ifndef MATH_H
#define MATH_H

#include "types.h"
#include <math.h>

void mat4_identity(f32* m);
void mat4_perspective(f32* m, f32 fov, f32 aspect, f32 near, f32 far);
void mat4_translate(f32* m, f32 x, f32 y, f32 z);
void mat4_rotate_x(f32* m, f32 angle);
void mat4_rotate_y(f32* m, f32 angle);
void mat4_rotate_z(f32* m, f32 angle);
void mat4_multiply(f32* res, f32* a, f32* b);
void mat4_lookat(f32* m, vec3s eye, vec3s center, vec3s up);

vec3s vec3_add(vec3s a, vec3s b);
vec3s vec3_sub(vec3s a, vec3s b);
vec3s vec3_scale(vec3s a, f32 s);
vec3s vec3_normalize(vec3s a);
vec3s vec3_cross(vec3s a, vec3s b);
f32 vec3_dot(vec3s a, vec3s b);

#endif
