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

void mat4_ortho(f32* m, const f32 left, const f32 right, const f32 bottom, const f32 top, const f32 near, const f32 far) {
    memset(m, 0, 16 * sizeof(f32));
    m[0] = 2.0f / (right - left);
    m[5] = 2.0f / (top - bottom);
    m[10] = -2.0f / (far - near);
    m[12] = -(right + left) / (right - left);
    m[13] = -(top + bottom) / (top - bottom);
    m[14] = -(far + near) / (far - near);
    m[15] = 1.0f;
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

void mat4_transpose(f32* m) {
    f32 tmp;
    tmp = m[1]; m[1] = m[4]; m[4] = tmp;
    tmp = m[2]; m[2] = m[8]; m[8] = tmp;
    tmp = m[3]; m[3] = m[12]; m[12] = tmp;
    tmp = m[6]; m[6] = m[9]; m[9] = tmp;
    tmp = m[7]; m[7] = m[10]; m[10] = tmp;
    tmp = m[11]; m[11] = m[14]; m[14] = tmp;
}

bool mat4_inverse(const f32* m, f32* out) {
    f32 tmp[16];
    tmp[0] = m[5]  * m[10] * m[15] - m[5]  * m[11] * m[14] - m[9]  * m[6]  * m[15] + m[9]  * m[7]  * m[14] + m[13] * m[6]  * m[11] - m[13] * m[7]  * m[10];
    tmp[4] = -m[4]  * m[10] * m[15] + m[4]  * m[11] * m[14] + m[8]  * m[6]  * m[15] - m[8]  * m[7]  * m[14] - m[12] * m[6]  * m[11] + m[12] * m[7]  * m[10];
    tmp[8] = m[4]  * m[9]  * m[15] - m[4]  * m[11] * m[13] - m[8]  * m[5]  * m[15] + m[8]  * m[7]  * m[13] + m[12] * m[5]  * m[11] - m[12] * m[7]  * m[9];
    tmp[12] = -m[4]  * m[9]  * m[14] + m[4]  * m[10] * m[13] + m[8]  * m[5]  * m[14] - m[8]  * m[6]  * m[13] - m[12] * m[5]  * m[10] + m[12] * m[6]  * m[9];
    tmp[1] = -m[1]  * m[10] * m[15] + m[1]  * m[11] * m[14] + m[9]  * m[2]  * m[15] - m[9]  * m[3]  * m[14] - m[13] * m[2]  * m[11] + m[13] * m[3]  * m[10];
    tmp[5] = m[0]  * m[10] * m[15] - m[0]  * m[11] * m[14] - m[8]  * m[2]  * m[15] + m[8]  * m[3]  * m[14] + m[12] * m[2]  * m[11] - m[12] * m[3]  * m[10];
    tmp[9] = -m[0]  * m[9]  * m[15] + m[0]  * m[11] * m[13] + m[8]  * m[1]  * m[15] - m[8]  * m[3]  * m[13] - m[12] * m[1]  * m[11] + m[12] * m[3]  * m[9];
    tmp[13] = m[0]  * m[9]  * m[14] - m[0]  * m[10] * m[13] - m[8]  * m[1]  * m[14] + m[8]  * m[2]  * m[13] + m[12] * m[1]  * m[10] - m[12] * m[2]  * m[9];
    tmp[2] = m[1]  * m[6]  * m[15] - m[1]  * m[7]  * m[14] - m[5]  * m[2]  * m[15] + m[5]  * m[3]  * m[14] + m[13] * m[2]  * m[7]  - m[13] * m[3]  * m[6];
    tmp[6] = -m[0]  * m[6]  * m[15] + m[0]  * m[7]  * m[14] + m[4]  * m[2]  * m[15] - m[4]  * m[3]  * m[14] - m[12] * m[2]  * m[7]  + m[12] * m[3]  * m[6];
    tmp[10] = m[0]  * m[5]  * m[15] - m[0]  * m[7]  * m[13] - m[4]  * m[1]  * m[15] + m[4]  * m[3]  * m[13] + m[12] * m[1]  * m[7]  - m[12] * m[3]  * m[5];
    tmp[14] = -m[0]  * m[5]  * m[14] + m[0]  * m[6]  * m[13] + m[4]  * m[1]  * m[14] - m[4]  * m[2]  * m[13] - m[12] * m[1]  * m[6]  + m[12] * m[2]  * m[5];
    tmp[3] = -m[1]  * m[6]  * m[11] + m[1]  * m[7]  * m[10] + m[5]  * m[2]  * m[11] - m[5]  * m[3]  * m[10] - m[9]  * m[2]  * m[7]  + m[9]  * m[3]  * m[6];
    tmp[7] = m[0]  * m[6]  * m[11] - m[0]  * m[7]  * m[10] - m[4]  * m[2]  * m[11] + m[4]  * m[3]  * m[10] + m[8]  * m[2]  * m[7]  - m[8]  * m[3]  * m[6];
    tmp[11] = -m[0]  * m[5]  * m[11] + m[0]  * m[7]  * m[9]  + m[4]  * m[1]  * m[11] - m[4]  * m[3]  * m[9]  - m[8]  * m[1]  * m[7]  + m[8]  * m[3]  * m[5];
    tmp[15] = m[0]  * m[5]  * m[10] - m[0]  * m[6]  * m[9]  - m[4]  * m[1]  * m[10] + m[4]  * m[2]  * m[9]  + m[8]  * m[1]  * m[6]  - m[8]  * m[2]  * m[5];

    f32 det = m[0] * tmp[0] + m[1] * tmp[4] + m[2] * tmp[8] + m[3] * tmp[12];
    if (fabsf(det) < 0.0001f) return false;
    
    f32 inv_det = 1.0f / det;
    for (i32 i = 0; i < 16; i++) out[i] = tmp[i] * inv_det;
    return true;
}

vec3s vec3_add(const vec3s a, const vec3s b) { return (vec3s){a.x + b.x, a.y + b.y, a.z + b.z}; }
vec3s vec3_sub(const vec3s a, const vec3s b) { return (vec3s){a.x - b.x, a.y - b.y, a.z - b.z}; }
vec3s vec3_scale(const vec3s a, const f32 s) { return (vec3s){a.x * s, a.y * s, a.z * s}; }
f32 vec3_dot(const vec3s a, const vec3s b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
f32 vec3_magnitude(const vec3s a) { return sqrtf(vec3_dot(a, a)); }

vec3s vec3_normalize(const vec3s a) {
    const f32 mag = vec3_magnitude(a);
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

vec2s vec2_add(const vec2s a, const vec2s b) { return (vec2s){a.x + b.x, a.y + b.y}; }
vec2s vec2_sub(const vec2s a, const vec2s b) { return (vec2s){a.x - b.x, a.y - b.y}; }
vec2s vec2_scale(const vec2s a, const f32 s) { return (vec2s){a.x * s, a.y * s}; }

f32 lerp(const f32 a, const f32 b, const f32 t) { return a + (b - a) * t; }
vec2s vec2_lerp(const vec2s a, const vec2s b, const f32 t) { return (vec2s){lerp(a.x, b.x, t), lerp(a.y, b.y, t)}; }
vec3s vec3_lerp(const vec3s a, const vec3s b, const f32 t) { return (vec3s){lerp(a.x, b.x, t), lerp(a.y, b.y, t), lerp(a.z, b.z, t)}; }
