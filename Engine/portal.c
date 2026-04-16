#include "portal.h"
#include "state.h"
#include "gfx.h"
#include <glad/glad.h>
#include "util/math.h"
#include <string.h>

static u32 g_portal_vao = 0;
static u32 g_portal_vbo = 0;
static u32 g_portal_ebo = 0;
static bool g_portal_vao_initialized = false;

static void ensure_portal_vao()
{
    if (g_portal_vao_initialized) return;

    glGenVertexArrays(1, &g_portal_vao);
    glGenBuffers(1, &g_portal_vbo);
    glGenBuffers(1, &g_portal_ebo);

    glBindVertexArray(g_portal_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_portal_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_portal_ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    g_portal_vao_initialized = true;
}

static void get_quad_model_matrix(const level_quad_t* quad, f32* out_model)
{
    f32 rot_y[16], rot_x[16], rot_z[16], temp[16];
    mat4_rotate_y(rot_y, -DEG2RAD(quad->rot.y));
    mat4_rotate_x(rot_x, -DEG2RAD(quad->rot.x));
    mat4_rotate_z(rot_z, -DEG2RAD(quad->rot.z));
    mat4_multiply(temp, rot_y, rot_x);
    mat4_multiply(out_model, temp, rot_z);
    out_model[12] = quad->pos.x;
    out_model[13] = quad->pos.y;
    out_model[14] = quad->pos.z;
}

static vec3s get_quad_normal(const f32* model)
{
    return (vec3s){
        model[0] * 0.0f + model[4] * 0.0f + model[8] * 1.0f,
        model[1] * 0.0f + model[5] * 0.0f + model[9] * 1.0f,
        model[2] * 0.0f + model[6] * 0.0f + model[10] * 1.0f
    };
}

static void get_quad_center(const level_quad_t* quad, vec3s* out_center)
{
    out_center->x = quad->pos.x + (quad->size.x * 0.5f);
    out_center->y = quad->pos.y + (quad->size.y * 0.5f);
    out_center->z = quad->pos.z;
}

static void apply_oblique_clipping(const level_quad_t* dest_quad, const f32* view_mat, f32* proj_mat)
{
    f32 dest_model[16];
    get_quad_model_matrix(dest_quad, dest_model);
    
    vec3s dest_center;
    get_quad_center(dest_quad, &dest_center);
    
    vec3s normal = get_quad_normal(dest_model);
    
    f32 d = -(normal.x * dest_center.x + normal.y * dest_center.y + normal.z * dest_center.z);
    
    vec4s clip_plane = {normal.x, normal.y, normal.z, d};
    
    f32 inv_view[16];
    if (!mat4_inverse(view_mat, inv_view)) return;
    mat4_transpose(inv_view);
    
    vec4s clip_eye;
    clip_eye.x = inv_view[0] * clip_plane.x + inv_view[1] * clip_plane.y + inv_view[2] * clip_plane.z + inv_view[3] * clip_plane.w;
    clip_eye.y = inv_view[4] * clip_plane.x + inv_view[5] * clip_plane.y + inv_view[6] * clip_plane.z + inv_view[7] * clip_plane.w;
    clip_eye.z = inv_view[8] * clip_plane.x + inv_view[9] * clip_plane.y + inv_view[10] * clip_plane.z + inv_view[11] * clip_plane.w;
    clip_eye.w = inv_view[12] * clip_plane.x + inv_view[13] * clip_plane.y + inv_view[14] * clip_plane.z + inv_view[15] * clip_plane.w;
    
    if (clip_eye.w > 0.0f) return;
    
    f32 inv_proj[16];
    if (!mat4_inverse(proj_mat, inv_proj)) return;
    
    vec4s q;
    q.x = inv_proj[0] * 1.0f + inv_proj[4] * 1.0f + inv_proj[8] * 1.0f + inv_proj[12] * 1.0f;
    q.y = inv_proj[1] * (-1.0f) + inv_proj[5] * (-1.0f) + inv_proj[9] * (-1.0f) + inv_proj[13] * 1.0f;
    q.z = inv_proj[2] * 1.0f + inv_proj[6] * 1.0f + inv_proj[10] * 1.0f + inv_proj[14] * 1.0f;
    q.w = inv_proj[3] * 1.0f + inv_proj[7] * 1.0f + inv_proj[11] * 1.0f + inv_proj[15] * 1.0f;
    
    q.x = (q.x >= 0) ? 1.0f : -1.0f;
    q.y = (q.y >= 0) ? 1.0f : -1.0f;
    q.z = 1.0f;
    q.w = 1.0f;
    
    f32 dot_prod = clip_eye.x * q.x + clip_eye.y * q.y + clip_eye.z * q.z + clip_eye.w * q.w;
    if (fabsf(dot_prod) < 0.0001f) return;
    
    vec4s c;
    c.x = clip_eye.x * (2.0f / dot_prod);
    c.y = clip_eye.y * (2.0f / dot_prod);
    c.z = clip_eye.z * (2.0f / dot_prod);
    c.w = clip_eye.w * (2.0f / dot_prod);
    
    f32 row3_x = proj_mat[3];
    f32 row3_y = proj_mat[7];
    f32 row3_z = proj_mat[11];
    f32 row3_w = proj_mat[15];
    
    proj_mat[8] = c.x - row3_x;
    proj_mat[9] = c.y - row3_y;
    proj_mat[10] = c.z - row3_z;
    proj_mat[11] = c.w - row3_w;
}

static void render_quad_geometry(const level_quad_t* quad, const vec4s color)
{
    ensure_portal_vao();

    f32 model[16];
    get_quad_model_matrix(quad, model);

    GLint model_loc = glGetUniformLocation(state.data->program, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);

    if (quad->tex_idx >= 0 && quad->tex_idx < state.text->count) texture_bind(&state.text->textures[quad->tex_idx], 0);
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

    glBindBuffer(GL_ARRAY_BUFFER, g_portal_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_portal_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(g_portal_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void render_portal(level_quad_t* source, level_quad_t* dest)
{
    f32 view[16], proj[16];
    camera_t* cam = state.cam;
    
    vec3s cam_front = cam->front;
    vec3s cam_up = cam->up;
    vec3s look_at = {cam->pos.x + cam_front.x, cam->pos.y + cam_front.y, cam->pos.z + cam_front.z};
    mat4_lookat(view, cam->pos, look_at, cam_up);
    
    int win_w, win_h;
    glfwGetFramebufferSize(state.win, &win_w, &win_h);
    f32 aspect = (f32)win_w / (f32)win_h;
    mat4_perspective(proj, DEG2RAD(45.0f), aspect, 0.1f, 1000.0f);
    
    f32 source_model[16], dest_model[16];
    get_quad_model_matrix(source, source_model);
    get_quad_model_matrix(dest, dest_model);
    
    f32 inv_source[16], inv_dest[16];
    if (!mat4_inverse(source_model, inv_source)) return;
    if (!mat4_inverse(dest_model, inv_dest)) return;
    
    f32 flip_y[16];
    mat4_identity(flip_y);
    flip_y[5] = -1.0f;
    
    f32 portal_transform[16], temp[16];
    mat4_multiply(temp, inv_source, view);
    mat4_multiply(portal_transform, inv_dest, temp);
    mat4_multiply(portal_transform, flip_y, portal_transform);
    
    vec3s source_center;
    get_quad_center(source, &source_center);
    
    f32 translation[16];
    mat4_translate(translation, source_center.x, source_center.y, source_center.z);
    f32 final_view[16];
    mat4_multiply(final_view, portal_transform, translation);
    
    f32 portal_proj[16];
    memcpy(portal_proj, proj, 16 * sizeof(f32));
    apply_oblique_clipping(dest, final_view, portal_proj);
    
    GLint view_loc = glGetUniformLocation(state.data->program, "view");
    GLint proj_loc = glGetUniformLocation(state.data->program, "projection");
    
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NEVER, 1, 0xFF);
    glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
    glStencilMask(0xFF);
    
    glClear(GL_STENCIL_BUFFER_BIT);
    
    vec4s white = {1.0f, 1.0f, 1.0f, 1.0f};
    render_quad_geometry(source, white);
    
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    
    glStencilMask(0x00);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, final_view);
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, portal_proj);
    
    level_render(state.levels);
    
    glDisable(GL_STENCIL_TEST);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    
    glClear(GL_DEPTH_BUFFER_BIT);
    render_quad_geometry(source, white);
    
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glEnable(GL_STENCIL_TEST);
}
