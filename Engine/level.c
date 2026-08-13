#include "level.h"
#include "state.h"

#include <glad/glad.h>
#include <math.h>

typedef struct {
    vec3s n;
    f32 d;
} plane_t;

static plane_t g_planes[6];
static bool g_frustum_set = false;
static u32 g_quad_vao = 0;
static u32 g_quad_vbo = 0;
static u32 g_quad_ebo = 0;

static bool sphere_inside_frustum(const vec3s center, const f32 radius)
{
    for (i32 i = 0; i < 6; ++i) {
        const f32 dist = vec3_dot(center, g_planes[i].n) + g_planes[i].d;
        if (dist + radius < 0.0f) return false;
    }
    return true;
}

void level_set_frustum(const f32* view, const f32* proj)
{
    if (!view || !proj) {
        g_frustum_set = false;
        return;
    }

    f32 m[16];
    mat4_multiply(m, proj, view);

    g_planes[0].n = (vec3s){m[3]+m[0], m[7]+m[4], m[11]+m[8]};  g_planes[0].d = m[15]+m[12];
    g_planes[1].n = (vec3s){m[3]-m[0], m[7]-m[4], m[11]-m[8]};  g_planes[1].d = m[15]-m[12];
    g_planes[2].n = (vec3s){m[3]+m[1], m[7]+m[5], m[11]+m[9]};  g_planes[2].d = m[15]+m[13];
    g_planes[3].n = (vec3s){m[3]-m[1], m[7]-m[5], m[11]-m[9]};  g_planes[3].d = m[15]-m[13];
    g_planes[4].n = (vec3s){m[3]+m[2], m[7]+m[6], m[11]+m[10]}; g_planes[4].d = m[15]+m[14];
    g_planes[5].n = (vec3s){m[3]-m[2], m[7]-m[6], m[11]-m[10]}; g_planes[5].d = m[15]-m[14];

    for (i32 i = 0; i < 6; ++i) {
        const f32 mag = vec3_magnitude(g_planes[i].n);
        if (mag > 0.0001f) {
            g_planes[i].n = vec3_scale(g_planes[i].n, 1.0f / mag);
            g_planes[i].d /= mag;
        }
    }
    g_frustum_set = true;
}

bool quad_visible(const level_quad_t* quad)
{
    if (!quad || !g_frustum_set) return quad != NULL;

    f32 rot_y[16], rot_x[16], rot_z[16], model[16], temp[16];
    mat4_rotate_y(rot_y, -DEG2RAD(quad->rot.y));
    mat4_rotate_x(rot_x, -DEG2RAD(quad->rot.x));
    mat4_rotate_z(rot_z, -DEG2RAD(quad->rot.z));
    mat4_multiply(temp, rot_x, rot_y);
    mat4_multiply(model, rot_z, temp);

    const vec3s lc = {quad->size.x * 0.5f, quad->size.y * 0.5f, 0.0f};
    const vec3s center = {
        model[0]*lc.x + model[4]*lc.y + model[8]*lc.z + quad->pos.x,
        model[1]*lc.x + model[5]*lc.y + model[9]*lc.z + quad->pos.y,
        model[2]*lc.x + model[6]*lc.y + model[10]*lc.z + quad->pos.z,
    };
    const f32 radius = sqrtf(quad->size.x*quad->size.x + quad->size.y*quad->size.y) * 0.5f;
    return sphere_inside_frustum(center, radius);
}

static bool ensure_quad_mesh(void)
{
    if (g_quad_vao) return true;

    static const f32 vertices[] = {
        1,1,0, 1,1,1, 1,1,
        1,0,0, 1,1,1, 1,0,
        0,0,0, 1,1,1, 0,0,
        0,1,0, 1,1,1, 0,1,
    };
    static const u32 indices[] = {0,1,3, 1,2,3};

    glGenVertexArrays(1, &g_quad_vao);
    glGenBuffers(1, &g_quad_vbo);
    glGenBuffers(1, &g_quad_ebo);
    if (!g_quad_vao || !g_quad_vbo || !g_quad_ebo) {
        level_renderer_shutdown();
        return false;
    }

    glBindVertexArray(g_quad_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_quad_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_quad_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));
    glBindVertexArray(0);
    return true;
}

void level_renderer_shutdown(void)
{
    if (g_quad_ebo) glDeleteBuffers(1, &g_quad_ebo);
    if (g_quad_vbo) glDeleteBuffers(1, &g_quad_vbo);
    if (g_quad_vao) glDeleteVertexArrays(1, &g_quad_vao);
    g_quad_ebo = 0;
    g_quad_vbo = 0;
    g_quad_vao = 0;
    g_frustum_set = false;
}

bool level_render_quad(const level_quad_t* quad, const vec4s color, const camera_t* cam)
{
    if (!quad || !g_state || !state.data || !state.text || !ensure_quad_mesh()) return false;

    f32 model[16];
    mat4_identity(model);

    if (quad->is_billboard && cam) {
        const vec3s delta = vec3_sub(cam->pos, quad->pos);
        if (vec3_magnitude(delta) > 0.0001f) {
            const vec3s to_cam = vec3_normalize(delta);
            const f32 yaw = RAD2DEG(atan2f(to_cam.x, to_cam.z));
            const f32 cy = cosf(-DEG2RAD(yaw));
            const f32 sy = sinf(-DEG2RAD(yaw));
            model[0] = cy;
            model[2] = sy;
            model[8] = -sy;
            model[10] = cy;
        }
    } else {
        f32 rot_y[16], rot_x[16], rot_z[16], temp[16];
        mat4_rotate_y(rot_y, -DEG2RAD(quad->rot.y));
        mat4_rotate_x(rot_x, -DEG2RAD(quad->rot.x));
        mat4_rotate_z(rot_z, -DEG2RAD(quad->rot.z));
        mat4_multiply(temp, rot_x, rot_y);
        mat4_multiply(model, rot_z, temp);
    }

    model[0] *= quad->size.x; model[1] *= quad->size.x; model[2] *= quad->size.x;
    model[4] *= quad->size.y; model[5] *= quad->size.y; model[6] *= quad->size.y;
    model[12] = quad->pos.x;
    model[13] = quad->pos.y;
    model[14] = quad->pos.z;

    glUniformMatrix4fv(state.data->u_model, 1, GL_FALSE, model);
    glUniform3f(state.data->u_tint, color.x, color.y, color.z);
    glUniform2f(state.data->u_uv_scale, quad->size.x * 0.5f, quad->size.y * 0.5f);

    if (quad->tex_id >= 0 && quad->tex_id < state.text->count)
        texture_bind(&state.text->textures[quad->tex_id], 0);
    else
        texture_bind(texture_get_fallback(), 0);

    glBindVertexArray(g_quad_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    return true;
}

void level_render(const level_data_t* level, const camera_t* cam)
{
    if (!level || !level->sectors) return;
    for (i32 i = 0; i < level->sector_count; ++i) {
        const level_sector_data_t* sector = &level->sectors[i];
        if (!sector->quads) continue;
        for (i32 j = 0; j < sector->quad_count; ++j) {
            const level_quad_t* quad = &sector->quads[j];
            if (quad->is_invisible || quad->portal_id > 0 || !quad_visible(quad)) continue;
            const vec4s color = {
                quad->color.x * sector->light.x,
                quad->color.y * sector->light.y,
                quad->color.z * sector->light.z,
                1.0f
            };
            level_render_quad(quad, color, cam);
        }
    }
}

bool level_ray_intersects_quad(const vec3s ray_origin, const vec3s ray_dir,
    const level_quad_t* quad, f32* out_t, vec3s* out_hit, vec3s* out_local_hit)
{
    if (!quad) return false;

    f32 rot_y[16], rot_x[16], rot_z[16], model[16], temp[16];
    mat4_rotate_y(rot_y, -DEG2RAD(quad->rot.y));
    mat4_rotate_x(rot_x, -DEG2RAD(quad->rot.x));
    mat4_rotate_z(rot_z, -DEG2RAD(quad->rot.z));
    mat4_multiply(temp, rot_x, rot_y);
    mat4_multiply(model, rot_z, temp);

    const vec3s normal = {model[8], model[9], model[10]};
    const f32 denom = vec3_dot(normal, ray_dir);
    if (fabsf(denom) < 0.0001f) return false;

    const f32 t = vec3_dot(vec3_sub(quad->pos, ray_origin), normal) / denom;
    if (t < 0.0f) return false;

    const vec3s hit = vec3_add(ray_origin, vec3_scale(ray_dir, t));
    const vec3s local_hit = vec3_sub(hit, quad->pos);

    f32 inv_rot_z[16], inv_rot_x[16], inv_rot_y[16], inv_model[16], temp2[16];
    mat4_rotate_z(inv_rot_z, DEG2RAD(quad->rot.z));
    mat4_rotate_x(inv_rot_x, DEG2RAD(quad->rot.x));
    mat4_rotate_y(inv_rot_y, DEG2RAD(quad->rot.y));
    mat4_multiply(temp2, inv_rot_x, inv_rot_z);
    mat4_multiply(inv_model, inv_rot_y, temp2);

    const vec3s local = {
        inv_model[0]*local_hit.x + inv_model[4]*local_hit.y + inv_model[8]*local_hit.z,
        inv_model[1]*local_hit.x + inv_model[5]*local_hit.y + inv_model[9]*local_hit.z,
        inv_model[2]*local_hit.x + inv_model[6]*local_hit.y + inv_model[10]*local_hit.z
    };

    if (local.x < -0.01f || local.x > quad->size.x + 0.01f ||
        local.y < -0.01f || local.y > quad->size.y + 0.01f)
        return false;

    if (out_t) *out_t = t;
    if (out_hit) *out_hit = hit;
    if (out_local_hit) *out_local_hit = local;
    return true;
}

bool level_get_height(const level_data_t* level, const vec3s pos, f32* h)
{
    if (!level || !level->sectors) return false;
    f32 best_height = -1000000.0f;
    bool found = false;

    for (i32 s = 0; s < level->sector_count; ++s) {
        const level_sector_data_t* sector = &level->sectors[s];
        if (!sector->quads) continue;
        for (i32 q = 0; q < sector->quad_count; ++q) {
            const level_quad_t* quad = &sector->quads[q];
            if (!quad->is_solid) continue;
            f32 t = 0.0f;
            vec3s hit = {0};
            if (level_ray_intersects_quad((vec3s){pos.x, 1000.0f, pos.z}, (vec3s){0,-1,0}, quad, &t, &hit, NULL) &&
                hit.y < pos.y - 1.0f && hit.y > best_height) {
                best_height = hit.y;
                found = true;
            }
        }
    }

    if (found && h) *h = best_height;
    return found;
}
