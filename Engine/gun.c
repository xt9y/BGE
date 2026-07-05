#include "gun.h"
#include "state.h"
#include <glad/glad.h>
#include <math.h>
#include <string.h>

static u32 g_vao = 0, g_vbo = 0;
static i32 g_current = 0;
static f32 g_swing = 0.0f, g_swing_vel = 0.0f;
static bool g_a_prev = false, g_d_prev = false;
static f32 g_flash_timer = 0.0f;
static f32 g_anim_time = 0.0f;

void gun_reg_init(gun_registry_t* reg)
{
    memset(reg, 0, sizeof(*reg));
    reg->count = 0;
}

void gun_init(void)
{
    vertex_t verts[6] = {
        {{0,0,0}, {0,0}, {1,1,1,1}},
        {{1,1,0}, {1,1}, {1,1,1,1}},
        {{1,0,0}, {1,0}, {1,1,1,1}},
        {{0,0,0}, {0,0}, {1,1,1,1}},
        {{0,1,0}, {0,1}, {1,1,1,1}},
        {{1,1,0}, {1,1}, {1,1,1,1}},
    };

    glGenVertexArrays(1, &g_vao);
    glGenBuffers(1, &g_vbo);
    glBindVertexArray(g_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(f32) * 3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(f32) * 5));
    glBindVertexArray(0);
}

void gun_shutdown(void)
{
    if (g_vbo) glDeleteBuffers(1, &g_vbo);
    if (g_vao) glDeleteVertexArrays(1, &g_vao);
    g_vbo = 0;
    g_vao = 0;
}

static i32 gun_current_frame(const weapon_def_t* w)
{
    if (w->frame_duration <= 0.0f || w->tex_count <= 1) return 0;
    i32 f = (i32)(g_anim_time / w->frame_duration);
    if (f >= w->tex_count) { g_anim_time = 0.0f; return 0; }
    return f;
}

void gun_render(i32 rw, i32 rh)
{
    if (state.id != STATE_PLAYING) return;
    if (!g_vao) return;
    if (g_current < 0 || g_current >= state.gun->count) return;

    const weapon_def_t* w = &state.gun->defs[g_current];

    i32 frame = gun_current_frame(w);
    const texture_t* tex = texture_get_by_name(w->tex_names[frame]);
    if (!tex || tex->width <= 0 || tex->height <= 0) return;

    if (g_anim_time > 0.0f) g_anim_time += state.dt;

    bool a_down = glfwGetKey(state.win, GLFW_KEY_A) == GLFW_PRESS;
    bool d_down = glfwGetKey(state.win, GLFW_KEY_D) == GLFW_PRESS;
    if (a_down && !g_a_prev) g_swing_vel = -120.0f;
    if (d_down && !g_d_prev) g_swing_vel = 120.0f;
    g_a_prev = a_down;
    g_d_prev = d_down;
    g_swing_vel += (-150.0f * g_swing - 12.0f * g_swing_vel) * state.dt;
    g_swing += g_swing_vel * state.dt;

    f32 t = (f32)glfwGetTime();
    f32 aspect = (f32)tex->width / (f32)tex->height;
    f32 gh = (f32)rh * w->gun_size;
    f32 gw = gh * aspect;
    f32 gx = (f32)rw * w->gun_xy.x - gw * 0.5f + sinf(t * 1.8f) * 3.0f + g_swing;
    f32 gy = (f32)rh * w->gun_xy.y - gh + cosf(t * 2.2f) * 2.5f;

    f32 proj[16];
    mat4_ortho(proj, 0.0f, (f32)rw, (f32)rh, 0.0f, -1.0f, 1.0f);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    u32 program = text_get_program();
    glUseProgram(program);
    glUniformMatrix4fv(glGetUniformLocation(program, "u_proj"), 1, GL_FALSE, proj);
    glUniform1i(glGetUniformLocation(program, "u_font"), 0);

    g_flash_timer -= state.dt;
    if (g_flash_timer > 0 && w->flash_tex_name) {
        const texture_t* flash = texture_get_by_name(w->flash_tex_name);
        if (flash && flash->width > 0) {
            f32 fs = w->flash_size * (f32)rh;
            f32 fx = gx + gw * w->flash_xy.x - fs * 0.5f;
            f32 fy = gy + gh * w->flash_xy.y - fs * 0.5f;
            vertex_t fverts[6] = {
                {{fx, fy, 0}, {0, 0}, {1,1,1,1}},
                {{fx+fs, fy+fs, 0}, {1, 1}, {1,1,1,1}},
                {{fx+fs, fy, 0}, {1, 0}, {1,1,1,1}},
                {{fx, fy, 0}, {0, 0}, {1,1,1,1}},
                {{fx, fy+fs, 0}, {0, 1}, {1,1,1,1}},
                {{fx+fs, fy+fs, 0}, {1, 1}, {1,1,1,1}},
            };
            texture_bind((texture_t*)flash, 0);
            glBindVertexArray(g_vao);
            glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(fverts), fverts);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    texture_bind((texture_t*)tex, 0);
    vertex_t verts[6] = {
        {{gx, gy, 0}, {0, 0}, {1,1,1,1}},
        {{gx+gw, gy+gh, 0}, {1, 1}, {1,1,1,1}},
        {{gx+gw, gy, 0}, {1, 0}, {1,1,1,1}},
        {{gx, gy, 0}, {0, 0}, {1,1,1,1}},
        {{gx, gy+gh, 0}, {0, 1}, {1,1,1,1}},
        {{gx+gw, gy+gh, 0}, {1, 1}, {1,1,1,1}},
    };

    glBindVertexArray(g_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

void gun_shot(void)
{
    if (state.id != STATE_PLAYING) return;
    g_flash_timer = 0.1f;
    g_anim_time = 0.001f;
}

i32 gun_get_current(void)
{
    return g_current;
}

void gun_select(i32 idx)
{
    if (idx >= 0 && idx < state.gun->count) g_current = idx;
}

void gun_next(void)
{
    g_current = (g_current + 1) % state.gun->count;
}

void gun_prev(void)
{
    g_current = (g_current - 1 + state.gun->count) % state.gun->count;
}
