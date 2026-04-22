#include "level.h"
#include "state.h"
#include "gfx.h"
#include <glad/glad.h>
#include "util/types.h"

#include <string.h>
#include <math.h>

static u32 g_wall_vao = 0;
static u32 g_wall_vbo = 0;
static u32 g_wall_ebo = 0;
static bool g_wall_vao_initialized = false;

static u32 g_billboard_vao = 0;
static u32 g_billboard_vbo = 0;
static u32 g_billboard_ebo = 0;
static bool g_billboard_vao_initialized = false;

static void ensure_vao()
{
    if (g_wall_vao_initialized) return;

    glGenVertexArrays(1, &g_wall_vao);
    glGenBuffers(1, &g_wall_vbo);
    glGenBuffers(1, &g_wall_ebo);

    glBindVertexArray(g_wall_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_wall_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_wall_ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    g_wall_vao_initialized = true;
}

static void ensure_billboard_vao()
{
    if (g_billboard_vao_initialized) return;

    glGenVertexArrays(1, &g_billboard_vao);
    glGenBuffers(1, &g_billboard_vbo);
    glGenBuffers(1, &g_billboard_ebo);

    glBindVertexArray(g_billboard_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_billboard_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_billboard_ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    g_billboard_vao_initialized = true;
}

void level_render_quad(const level_quad_t* quad, const vec4s color)
{
    ensure_vao();

    f32 rot_y[16], rot_x[16], rot_z[16];
    mat4_rotate_y(rot_y, -DEG2RAD(quad->rot.y));
    mat4_rotate_x(rot_x, -DEG2RAD(quad->rot.x));
    mat4_rotate_z(rot_z, -DEG2RAD(quad->rot.z));

    f32 temp[16], model[16];
    mat4_multiply(temp, rot_y, rot_x);
    mat4_multiply(model, temp, rot_z);

    model[12] = quad->pos.x;
    model[13] = quad->pos.y;
    model[14] = quad->pos.z;

    GLint model_loc = glGetUniformLocation(state.data->program, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);

    if (quad->tex_id >= 0 && quad->tex_id < state.text->count) texture_bind(&state.text->textures[quad->tex_id], 0);
    else texture_bind(texture_get_fallback(), 0);

    f32 u_repeat = quad->size.x * 0.5f, v_repeat = quad->size.y * 0.5f;

    f32 vertices[] = {
         quad->size.x,  quad->size.y, 0.0f,   color.x, color.y, color.z,   u_repeat, v_repeat,
         quad->size.x,  0.0f,         0.0f,   color.x, color.y, color.z,   u_repeat, 0.0f,
         0.0f,          0.0f,         0.0f,   color.x, color.y, color.z,   0.0f,     0.0f,
         0.0f,          quad->size.y, 0.0f,   color.x, color.y, color.z,   0.0f,     v_repeat,
    };

    u32 indices[] = {
        0, 1, 3, 1, 2, 3
    };

    glBindBuffer(GL_ARRAY_BUFFER, g_wall_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_wall_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(g_wall_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

static bool should_render_quad(const level_quad_t* quad, const level_render_options_t* options)
{
    if (!quad) return false;
    if (quad->is_invisible) return false;
    if (options && options->skip_quad == quad) return false;
    return true;
}

static void render_sector(const level_sector_data_t *sector, const level_render_options_t* options)
{
    for (i32 i = 0; i < sector->quad_count; i++) {
        if (!should_render_quad(&sector->quads[i], options)) continue;
        if (sector->quads[i].is_billboard) continue;

        const vec4s wall_color = {
            sector->quads[i].color.x * sector->light.x,
            sector->quads[i].color.y * sector->light.y,
            sector->quads[i].color.z * sector->light.z,
            1.0f
        };

        level_render_quad(&sector->quads[i], wall_color);
    }
}

static void render_billboard_quad(const level_quad_t* quad, const vec4s color, const vec3s cam_pos)
{
    ensure_billboard_vao();

    vec3s to_cam = vec3_normalize(vec3_sub(cam_pos, quad->pos));
    f32 yaw = atan2f(to_cam.x, to_cam.z) * 180.0f / M_PI;

    f32 model[16];
    mat4_identity(model);

    f32 cos_y = cosf(-DEG2RAD(yaw));
    f32 sin_y = sinf(-DEG2RAD(yaw));
    model[0] = cos_y;
    model[2] = sin_y;
    model[8] = -sin_y;
    model[10] = cos_y;

    model[12] = quad->pos.x;
    model[13] = quad->pos.y;
    model[14] = quad->pos.z;

    GLint model_loc = glGetUniformLocation(state.data->program, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);

    if (quad->tex_id >= 0 && quad->tex_id < state.text->count) texture_bind(&state.text->textures[quad->tex_id], 0);
    else texture_bind(texture_get_fallback(), 0);

    f32 u_repeat = quad->size.x * 0.5f, v_repeat = quad->size.y * 0.5f;

    f32 vertices[] = {
         quad->size.x,  quad->size.y, 0.0f,   color.x, color.y, color.z,   u_repeat, v_repeat,
         quad->size.x,  0.0f,         0.0f,   color.x, color.y, color.z,   u_repeat, 0.0f,
         0.0f,          0.0f,         0.0f,   color.x, color.y, color.z,   0.0f,     0.0f,
         0.0f,          quad->size.y, 0.0f,   color.x, color.y, color.z,   0.0f,     v_repeat,
    };

    u32 indices[] = {
        0, 1, 3, 1, 2, 3
    };

    glBindBuffer(GL_ARRAY_BUFFER, g_billboard_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_billboard_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(g_billboard_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void level_render_billboards_with_options(const level_data_t *level, const camera_t *cam, const level_render_options_t* options)
{
    for (i32 i = 0; i < level->sector_count; i++) {
        level_sector_data_t *sector = &level->sectors[i];
        for (i32 j = 0; j < sector->quad_count; j++) {
            level_quad_t *quad = &sector->quads[j];
            if (!quad->is_billboard || !should_render_quad(quad, options)) continue;

            const vec4s billboard_color = {
                quad->color.x * sector->light.x,
                quad->color.y * sector->light.y,
                quad->color.z * sector->light.z,
                1.0f
            };

            render_billboard_quad(quad, billboard_color, cam->pos);
        }
    }
}

void level_render_billboards(const level_data_t *level, const camera_t *cam)
{
    level_render_billboards_with_options(level, cam, NULL);
}

void level_render_with_options(const level_data_t *level, const level_render_options_t* options)
{
    for (i32 i = 0; i < level->sector_count; i++) render_sector(&level->sectors[i], options);
}

void level_render(const level_data_t *level)
{
    level_render_with_options(level, NULL);
}

bool level_ray_intersects_quad(const vec3s ray_origin, const vec3s ray_dir, const level_quad_t* quad, f32* out_t, vec3s* out_hit, vec3s* out_local_hit)
{
    f32 rot_y[16], rot_x[16], rot_z[16], model[16], temp[16];
    mat4_rotate_y(rot_y, -DEG2RAD(quad->rot.y));
    mat4_rotate_x(rot_x, -DEG2RAD(quad->rot.x));
    mat4_rotate_z(rot_z, -DEG2RAD(quad->rot.z));
    mat4_multiply(temp, rot_y, rot_x);
    mat4_multiply(model, temp, rot_z);

    vec3s wall_pos = {
        quad->pos.x, 
        quad->pos.y, 
        quad->pos.z
    };

    vec3s normal = {
        model[0] * 0.0f + model[4] * 0.0f + model[8] * 1.0f,
        model[1] * 0.0f + model[5] * 0.0f + model[9] * 1.0f,
        model[2] * 0.0f + model[6] * 0.0f + model[10] * 1.0f
    };

    if (fabsf(vec3_dot(normal, ray_dir)) < 0.0001f) return false;

    f32 t = vec3_dot(vec3_sub(wall_pos, ray_origin), normal) / vec3_dot(normal, ray_dir);
    if (t < 0.0f) return false;

    vec3s hit = vec3_add(ray_origin, vec3_scale(ray_dir, t)), local_hit = vec3_sub(hit, wall_pos);

    f32 inv_rot_z[16], inv_rot_x[16], inv_rot_y[16], inv_model[16], temp2[16];
    mat4_rotate_z(inv_rot_z, DEG2RAD(quad->rot.z));
    mat4_rotate_x(inv_rot_x, DEG2RAD(quad->rot.x));
    mat4_rotate_y(inv_rot_y, DEG2RAD(quad->rot.y));
    mat4_multiply(temp2, inv_rot_z, inv_rot_x);
    mat4_multiply(inv_model, temp2, inv_rot_y);

    vec3s unrotated_hit = {
        inv_model[0] * local_hit.x + inv_model[4] * local_hit.y + inv_model[8] * local_hit.z,
        inv_model[1] * local_hit.x + inv_model[5] * local_hit.y + inv_model[9] * local_hit.z,
        inv_model[2] * local_hit.x + inv_model[6] * local_hit.y + inv_model[10] * local_hit.z
    };

    if (unrotated_hit.x >= -0.01f && unrotated_hit.x <= quad->size.x + 0.01f && 
        unrotated_hit.y >= -0.01f && unrotated_hit.y <= quad->size.y + 0.01f) {
        if (out_t) *out_t = t;
        if (out_hit) *out_hit = hit;
        if (out_local_hit) *out_local_hit = unrotated_hit;
        return true;
    }
    return false;
}

bool level_get_height(const level_data_t* level, vec3s pos, f32* h)
{
    f32 best_height = -1000000.0f;
    bool found = false;
    for (i32 s = 0; s < level->sector_count; s++)
        for (i32 q = 0; q < level->sectors[s].quad_count; q++) {
            if (!level->sectors[s].quads[q].is_solid) continue;
            f32 t; vec3s hit;
            if (level_ray_intersects_quad((vec3s){pos.x, 1000.0f, pos.z}, (vec3s){0, -1, 0}, &level->sectors[s].quads[q], &t, &hit, NULL))
                if (hit.y < pos.y - 1.0f) if (hit.y > best_height) { best_height = hit.y; found = true; }
        }
    if (found && h) *h = best_height;
    return found;
}
