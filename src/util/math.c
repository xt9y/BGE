#include "math.h"
#include <string.h>

void mat4_identity(f32* m) {
    memset(m, 0, 16 * sizeof(f32));
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void mat4_perspective(f32* m, const f32 fov, const f32 aspect, const f32 near, const f32 far) {
    const f32 x = tanf(fov / 2.0f);
    memset(m, 0, 16 * sizeof(f32));
    m[0] = 1.0f / (aspect * x);
    m[5] = 1.0f / x;
    m[10] = -(far + near) / (far - near);
    m[11] = -1.0f;
    m[14] = -(2.0f * far * near) / (far - near);
}

void mat4_translate(f32* m, const f32 x, const f32 y, const f32 z) {
    mat4_identity(m);
    m[12] = x;
    m[13] = y;
    m[14] = z;
}

void mat4_rotate_x(f32* m, const f32 angle) {
    mat4_identity(m);
    const f32 c = cosf(angle);
    const f32 s = sinf(angle);
    m[5] = c;
    m[6] = s;
    m[9] = -s;
    m[10] = c;
}

void mat4_rotate_y(f32* m, const f32 angle) {
    mat4_identity(m);
    const f32 c = cosf(angle);
    const f32 s = sinf(angle);
    m[0] = c;
    m[2] = -s;
    m[8] = s;
    m[10] = c;
}

void mat4_rotate_z(f32* m, const f32 angle) {
    mat4_identity(m);
    const f32 c = cosf(angle);
    const f32 s = sinf(angle);
    m[0] = c;
    m[1] = s;
    m[4] = -s;
    m[5] = c;
}

void mat4_multiply(f32* res, const f32* a, const f32* b) {
    f32 tmp[16];
    for (i32 i = 0; i < 4; i++)
    for (i32 j = 0; j < 4; j++)
        tmp[i * 4 + j] = a[i * 4 + 0] * b[0 * 4 + j] +
                         a[i * 4 + 1] * b[1 * 4 + j] +
                         a[i * 4 + 2] * b[2 * 4 + j] +
                         a[i * 4 + 3] * b[3 * 4 + j];
    memcpy(res, tmp, 16 * sizeof(f32));
}

void mat4_lookat(f32* m, const vec3s eye, const vec3s center, const vec3s up) {
    const vec3s f = vec3_normalize(vec3_sub(center, eye));
    const vec3s s = vec3_normalize(vec3_cross(f, up));
    const vec3s u = vec3_cross(s, f);

    mat4_identity(m);
    m[0] = s.x;
    m[4] = s.y;
    m[8] = s.z;
    m[1] = u.x;
    m[5] = u.y;
    m[9] = u.z;
    m[2] = -f.x;
    m[6] = -f.y;
    m[10] = -f.z;
    m[12] = -vec3_dot(s, eye);
    m[13] = -vec3_dot(u, eye);
    m[14] = vec3_dot(f, eye);
}

vec3s vec3_add(const vec3s a, const vec3s b) { return (vec3s){a.x + b.x, a.y + b.y, a.z + b.z}; }
vec3s vec3_sub(const vec3s a, const vec3s b) { return (vec3s){a.x - b.x, a.y - b.y, a.z - b.z}; }
vec3s vec3_scale(const vec3s a, const f32 s) { return (vec3s){a.x * s, a.y * s, a.z * s}; }
f32 vec3_dot(const vec3s a, const vec3s b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

vec3s vec3_normalize(const vec3s a) {
    const f32 mag = sqrtf(vec3_dot(a, a));
    if (mag == 0) return a;
    return vec3_scale(a, 1.0f / mag);
}

vec3s vec3_cross(const vec3s a, const vec3s b) {
    return (vec3s){
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}
