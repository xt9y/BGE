#include "render.h"

#include "platform.h"
#include "editor.h"
#include "portal.h"
#include "state.h"

#include <glad/glad.h>
#include <math.h>
#include <string.h>

#define MAX_DEBUG_TRACES 64

typedef struct {
    vec3s points[MAX_DEBUG_TRACE_POINTS];
    i32 point_count;
    vec3s normal;
    vec3s color;
    f32 time;
    bool active;
} debug_trace_t;

static debug_trace_t g_debug_traces[MAX_DEBUG_TRACES];
static u32 g_debug_vao = 0;
static u32 g_debug_vbo = 0;

static vec3s quad_world_normal(const level_quad_t* q)
{
    f32 ry[16], rx[16], rz[16], m[16], t[16];
    mat4_rotate_y(ry, -DEG2RAD(q->rot.y));
    mat4_rotate_x(rx, -DEG2RAD(q->rot.x));
    mat4_rotate_z(rz, -DEG2RAD(q->rot.z));
    mat4_multiply(t, rx, ry);
    mat4_multiply(m, rz, t);
    return vec3_normalize((vec3s){m[8], m[9], m[10]});
}

bool render_init(void)
{
    memset(g_debug_traces, 0, sizeof(g_debug_traces));
    glGenVertexArrays(1, &g_debug_vao);
    glGenBuffers(1, &g_debug_vbo);
    if (!g_debug_vao || !g_debug_vbo) {
        render_shutdown();
        return false;
    }

    glBindVertexArray(g_debug_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_debug_vbo);
    glBufferData(GL_ARRAY_BUFFER, MAX_DEBUG_TRACES * 32 * 8 * (i32)sizeof(f32), NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));
    glBindVertexArray(0);
    return true;
}

void render_shutdown(void)
{
    if (g_debug_vbo) glDeleteBuffers(1, &g_debug_vbo);
    if (g_debug_vao) glDeleteVertexArrays(1, &g_debug_vao);
    g_debug_vbo = 0;
    g_debug_vao = 0;
    memset(g_debug_traces, 0, sizeof(g_debug_traces));
}

void render_debug_trace_add(const vec3s* points, i32 point_count, const vec3s normal, const vec3s color)
{
    if (!points || point_count < 2) return;
    if (point_count > MAX_DEBUG_TRACE_POINTS) point_count = MAX_DEBUG_TRACE_POINTS;

    i32 slot = -1;
    f32 oldest = 0.0f;
    for (i32 i = 0; i < MAX_DEBUG_TRACES; ++i) {
        if (!g_debug_traces[i].active) { slot = i; break; }
        if (slot < 0 || g_debug_traces[i].time < oldest) {
            slot = i;
            oldest = g_debug_traces[i].time;
        }
    }
    if (slot < 0) return;

    debug_trace_t* trace = &g_debug_traces[slot];
    memset(trace, 0, sizeof(*trace));
    memcpy(trace->points, points, sizeof(*points) * (size_t)point_count);
    trace->point_count = point_count;
    trace->normal = normal;
    trace->color = color;
    trace->time = (f32)glfwGetTime();
    trace->active = true;
}

static void render_debug_traces(void)
{
    if (!g_debug_vao || !g_debug_vbo) return;

    const f32 now = (f32)glfwGetTime();
    f32 verts[MAX_DEBUG_TRACES * 32 * 8];
    i32 line_verts = 0;
    i32 tri_verts = 0;

    for (i32 i = 0; i < MAX_DEBUG_TRACES; ++i) {
        debug_trace_t* trace = &g_debug_traces[i];
        if (!trace->active) continue;
        if (now - trace->time > 4.0f) {
            trace->active = false;
            continue;
        }

        if (state.debug_visible) {
            for (i32 j = 0; j + 1 < trace->point_count; j += 2) {
                const vec3s a = trace->points[j];
                const vec3s b = trace->points[j + 1];
                const vec3s c = trace->color;
                i32 n = line_verts++;
                verts[n*8+0]=a.x; verts[n*8+1]=a.y; verts[n*8+2]=a.z;
                verts[n*8+3]=c.x; verts[n*8+4]=c.y; verts[n*8+5]=c.z;
                verts[n*8+6]=0; verts[n*8+7]=0;
                n = line_verts++;
                verts[n*8+0]=b.x; verts[n*8+1]=b.y; verts[n*8+2]=b.z;
                verts[n*8+3]=c.x; verts[n*8+4]=c.y; verts[n*8+5]=c.z;
                verts[n*8+6]=0; verts[n*8+7]=0;
            }
        }
    }

    for (i32 i = 0; i < MAX_DEBUG_TRACES; ++i) {
        const debug_trace_t* trace = &g_debug_traces[i];
        if (!trace->active || now - trace->time > 4.0f || trace->point_count <= 0) continue;

        const vec3s n = trace->normal;
        const vec3s h = trace->points[trace->point_count - 1];
        vec3s up = {0,1,0};
        if (fabsf(vec3_dot(n, up)) > 0.99f) up = (vec3s){0,0,1};
        const vec3s right = vec3_normalize(vec3_cross(n, up));
        const vec3s bitangent = vec3_cross(n, right);
        const f32 size = 0.12f;
        const vec3s corners[4] = {
            vec3_add(vec3_add(h, vec3_scale(right,-size)), vec3_scale(bitangent,-size)),
            vec3_add(vec3_add(h, vec3_scale(right, size)), vec3_scale(bitangent,-size)),
            vec3_add(vec3_add(h, vec3_scale(right, size)), vec3_scale(bitangent, size)),
            vec3_add(vec3_add(h, vec3_scale(right,-size)), vec3_scale(bitangent, size)),
        };
        static const i32 idx[6] = {0,1,2, 0,2,3};
        const vec3s color = state.debug_visible ? trace->color : (vec3s){0,0,0};
        for (i32 j = 0; j < 6; ++j) {
            const vec3s p = corners[idx[j]];
            const i32 v = line_verts + tri_verts++;
            verts[v*8+0]=p.x; verts[v*8+1]=p.y; verts[v*8+2]=p.z;
            verts[v*8+3]=color.x; verts[v*8+4]=color.y; verts[v*8+5]=color.z;
            verts[v*8+6]=0; verts[v*8+7]=0;
        }
    }

    const i32 total = line_verts + tri_verts;
    if (!total) return;

    glDepthFunc(GL_LEQUAL);
    glUseProgram(state.data->program);
    f32 identity[16];
    mat4_identity(identity);
    glUniformMatrix4fv(state.data->u_model, 1, GL_FALSE, identity);
    glUniform3f(state.data->u_tint, 1, 1, 1);
    glUniform2f(state.data->u_uv_scale, 1, 1);
    texture_bind(texture_get_fallback(), 0);

    glBindVertexArray(g_debug_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_debug_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)(total * 8 * (i32)sizeof(f32)), verts);
    if (line_verts) glDrawArrays(GL_LINES, 0, line_verts);
    if (tri_verts) glDrawArrays(GL_TRIANGLES, line_verts, tri_verts);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void apply_level_camera(camera_t* cam, const level_data_t* level)
{
    if (!cam || !level) return;
    cam->pos = level->cam.pos;
    cam->yaw = level->cam.yaw;
    cam->pitch = level->cam.pitch;
    cam->firstMouse = true;
    update_camera_vectors(cam);
}

static void oblique_near_clip(f32* proj, const f32* view, const vec3s plane_pos, vec3s plane_normal)
{
    const vec3s pos_v = {
        view[0]*plane_pos.x + view[4]*plane_pos.y + view[8]*plane_pos.z + view[12],
        view[1]*plane_pos.x + view[5]*plane_pos.y + view[9]*plane_pos.z + view[13],
        view[2]*plane_pos.x + view[6]*plane_pos.y + view[10]*plane_pos.z + view[14]
    };
    vec3s norm_v = {
        view[0]*plane_normal.x + view[4]*plane_normal.y + view[8]*plane_normal.z,
        view[1]*plane_normal.x + view[5]*plane_normal.y + view[9]*plane_normal.z,
        view[2]*plane_normal.x + view[6]*plane_normal.y + view[10]*plane_normal.z
    };

    f32 d = -vec3_dot(norm_v, pos_v);
    if (norm_v.z > 0.0f) { norm_v = vec3_scale(norm_v, -1.0f); d = -d; }
    if (fabsf(d) < 0.005f) return;

    const vec4s cp = {norm_v.x, norm_v.y, norm_v.z, d};
    const f32 sx = cp.x > 0.0f ? 1.0f : (cp.x < 0.0f ? -1.0f : 0.0f);
    const f32 sy = cp.y > 0.0f ? 1.0f : (cp.y < 0.0f ? -1.0f : 0.0f);
    const vec4s q = {(sx + proj[8]) / proj[0], (sy + proj[9]) / proj[5], -1.0f,
                     (1.0f + proj[10]) / proj[14]};
    const f32 dot = cp.x*q.x + cp.y*q.y + cp.z*q.z + cp.w*q.w;
    if (fabsf(dot) < 0.0001f) return;
    const f32 scale = 2.0f / dot;
    proj[2] = cp.x * scale;
    proj[6] = cp.y * scale;
    proj[10] = cp.z * scale + 1.0f;
    proj[14] = cp.w * scale;
}

static bool set_camera_uniforms(const camera_t* cam)
{
    if (!cam || state.fb->w <= 0 || state.fb->h <= 0) return false;
    f32 view[16], proj[16];
    mat4_lookat(view, cam->pos, vec3_add(cam->pos, cam->front), cam->up);
    mat4_perspective(proj, DEG2RAD(45.0f), (f32)state.fb->w / (f32)state.fb->h, 0.1f, 100.0f);
    glUniformMatrix4fv(state.data->u_view, 1, GL_FALSE, view);
    glUniformMatrix4fv(state.data->u_proj, 1, GL_FALSE, proj);
    level_set_frustum(view, proj);
    return true;
}

static void restore_world_state(const i32 stencil_ref)
{
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthRange(0.0, 1.0);
    glStencilMask(0x00);
    glStencilFunc(GL_EQUAL, stencil_ref, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glUniform3f(state.data->u_tint, 1, 1, 1);
    glUniform2f(state.data->u_uv_scale, 1, 1);
}

static void render_portals(const level_data_t* level, const camera_t* cam, const i32 depth, const i32 stencil_ref)
{
    if (!level || !cam || depth >= MAX_PORTAL_DEPTH) return;

    for (i32 s = 0; s < level->sector_count; ++s) {
        const level_sector_data_t* sector = &level->sectors[s];
        if (!sector->quads) continue;
        for (i32 q = 0; q < sector->quad_count; ++q) {
            const level_quad_t* quad = &sector->quads[q];
            if (quad->portal_id <= 0) continue;

            portal_link_t link;
            camera_t portal_cam;
            if (!portal_find_link(level, quad, &link) || !quad_visible(link.src) ||
                !portal_build_camera(link.src, link.dst, cam, &portal_cam))
                continue;

            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            glDepthMask(GL_FALSE);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_CULL_FACE);
            glStencilMask(0xFF);
            glStencilFunc(GL_EQUAL, stencil_ref, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
            level_render_quad(link.src, (vec4s){1,1,1,1}, cam);

            glDepthMask(GL_TRUE);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_ALWAYS);
            glStencilMask(0x00);
            glStencilFunc(GL_EQUAL, stencil_ref + 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
            glDepthRange(1.0, 1.0);
            level_render_quad(link.src, (vec4s){1,1,1,1}, cam);
            glDepthRange(0.0, 1.0);
            glDepthFunc(GL_LESS);

            f32 view[16], proj[16];
            mat4_lookat(view, portal_cam.pos, vec3_add(portal_cam.pos, portal_cam.front), portal_cam.up);
            mat4_perspective(proj, DEG2RAD(45.0f), (f32)state.fb->w / (f32)state.fb->h, 0.1f, 100.0f);
            oblique_near_clip(proj, view, link.dst->pos, quad_world_normal(link.dst));
            glUniformMatrix4fv(state.data->u_view, 1, GL_FALSE, view);
            glUniformMatrix4fv(state.data->u_proj, 1, GL_FALSE, proj);
            level_set_frustum(view, proj);

            restore_world_state(stencil_ref + 1);
            render_portals(level, &portal_cam, depth + 1, stencil_ref + 1);
            restore_world_state(stencil_ref + 1);
            glUniformMatrix4fv(state.data->u_view, 1, GL_FALSE, view);
            glUniformMatrix4fv(state.data->u_proj, 1, GL_FALSE, proj);
            level_render(level, &portal_cam);
            render_debug_traces();

            set_camera_uniforms(cam);

            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
            glDepthMask(GL_TRUE);
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_ALWAYS);
            glStencilMask(0x00);
            glStencilFunc(GL_EQUAL, stencil_ref + 1, 0xFF);
            level_render_quad(link.src, (vec4s){1,1,1,1}, cam);

            glDepthFunc(GL_LESS);
            glDepthMask(GL_FALSE);
            glDisable(GL_DEPTH_TEST);
            glStencilMask(0xFF);
            glStencilFunc(GL_EQUAL, stencil_ref + 1, 0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
            level_render_quad(link.src, (vec4s){1,1,1,1}, cam);

            restore_world_state(stencil_ref);
        }
    }
}

bool render_main(const i32 rw, const i32 rh)
{
    if (!g_state || !state.editor || !state.editor->level || rw <= 0 || rh <= 0) return false;
    state.fb->w = rw;
    state.fb->h = rh;
    if (!fbo_resize(rw, rh)) return false;

    glBindFramebuffer(GL_FRAMEBUFFER, app_framebuffer());
    glViewport(0, 0, rw, rh);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xFF);
    glUseProgram(state.data->program);

    f32 identity[16];
    mat4_identity(identity);
    glUniformMatrix4fv(state.data->u_model, 1, GL_FALSE, identity);
    glUniform3f(state.data->u_tint, 1,1,1);
    glUniform2f(state.data->u_uv_scale, 1,1);
    if (!set_camera_uniforms(state.cam)) return false;

    render_portals(state.editor->level, state.cam, 0, 0);
    restore_world_state(0);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    level_render(state.editor->level, state.cam);
    if (state.id == STATE_EDITOR) editor_render_borders();
    render_debug_traces();
    return true;
}
