#include "level.h"
#include "state.h"
#include "gfx.h"
#include "util/types.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string.h>
#include <math.h>

static u32 g_wall_vao = 0;
static u32 g_wall_vbo = 0;
static u32 g_wall_ebo = 0;
static bool g_wall_vao_initialized = false;

static void ensure_wall_vao(void)
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

static void render_wall_quad(const level_quad_t* quad, const vec4s color)
{
    ensure_wall_vao();

    vec3s start = {
        quad->pos.x - (quad->size.x * 0.5f) * cosf(DEG2RAD(quad->rot.y)), quad->pos.y,
        quad->pos.z - (quad->size.x * 0.5f) * sinf(DEG2RAD(quad->rot.y))
    };

    vec3s end = {
        quad->pos.x + (quad->size.x * 0.5f) * cosf(DEG2RAD(quad->rot.y)), quad->pos.y,
        quad->pos.z + (quad->size.x * 0.5f) * sinf(DEG2RAD(quad->rot.y))
    };

    vec3s center = {
        (start.x + end.x) * 0.5f,
        (start.y + quad->size.y) * 0.5f,
        (start.z + end.z) * 0.5f
    };

    f32 length = sqrtf(
        (end.x - start.x) * (end.x - start.x) +
        (end.z - start.z) * (end.z - start.z)
    );

    f32 angle = atan2f(end.z - start.z, end.x - start.x) * 180.0f / PI;

    f32 rot_y_mat[16], model[16];
    mat4_rotate_y(rot_y_mat, DEG2RAD(angle));
    memcpy(model, rot_y_mat, sizeof(model));
    model[12] = center.x;
    model[13] = center.y;
    model[14] = center.z;

    GLint model_loc = glGetUniformLocation(state.data->program, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);

    if (quad->tex_idx >= 0 && quad->tex_idx < state.text->count) texture_bind(&state.text->textures[quad->tex_idx], 0);
    else texture_bind(texture_get_fallback(), 0);

    f32 half_x = 0.5f * length;
    f32 half_y = 0.5f * quad->size.y;
    f32 u_repeat = length * 0.5f, v_repeat = quad->size.y * 0.5f;

    f32 vertices[] = {
         half_x,  half_y, 0.0f,   color.x, color.y, color.z,   u_repeat, v_repeat,
         half_x, -half_y, 0.0f,   color.x, color.y, color.z,   u_repeat, 0.0f,
        -half_x, -half_y, 0.0f,   color.x, color.y, color.z,   0.0f,     0.0f,
        -half_x,  half_y, 0.0f,   color.x, color.y, color.z,   0.0f,     v_repeat,
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

static void render_sector(const level_sector_data_t *sector)
{
    for (i32 i = 0; i < sector->quad_count; i++)
    {
        const vec4s wall_color = {
            sector->quads[i].color.x * sector->light_intensity,
            sector->quads[i].color.y * sector->light_intensity,
            sector->quads[i].color.z * sector->light_intensity,
            1.0f
        };

        render_wall_quad(&sector->quads[i], wall_color);
    }
}

void level_render(const level_data_t *level)
{
    for (i32 i = 0; i < level->sector_count; i++) render_sector(&level->sectors[i]);
}
