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

    f32 rot_y[16], rot_x[16], rot_z[16];
    mat4_rotate_y(rot_y, -DEG2RAD(quad->rot.y));
    mat4_rotate_x(rot_x, -DEG2RAD(quad->rot.x));
    mat4_rotate_z(rot_z, -DEG2RAD(quad->rot.z));

    f32 temp[16], model[16];
    mat4_multiply(temp, rot_y, rot_x);
    mat4_multiply(model, temp, rot_z);

    model[12] = quad->pos.x;
    model[13] = quad->pos.y + quad->size.y * 0.5f;
    model[14] = quad->pos.z;

    GLint model_loc = glGetUniformLocation(state.data->program, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);

    if (quad->tex_idx >= 0 && quad->tex_idx < state.text->count) texture_bind(&state.text->textures[quad->tex_idx], 0);
    else texture_bind(texture_get_fallback(), 0);

    f32 half_x = 0.5f * quad->size.x;
    f32 half_y = 0.5f * quad->size.y;
    f32 u_repeat = quad->size.x * 0.5f, v_repeat = quad->size.y * 0.5f;

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
        if (sector->quads[i].is_invisible) continue;

        const vec4s wall_color = {
            sector->quads[i].color.x * sector->light.x,
            sector->quads[i].color.y * sector->light.y,
            sector->quads[i].color.z * sector->light.z,
            1.0f
        };

        render_wall_quad(&sector->quads[i], wall_color);
    }
}

void level_render(const level_data_t *level)
{
    for (i32 i = 0; i < level->sector_count; i++) render_sector(&level->sectors[i]);
}
