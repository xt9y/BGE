#include "editor.h"
#include "state.h"
#include "gfx.h"
#include "util/types.h"
#include "cam.h"
#include <glad/glad.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static const char *point_names[] = {
    "Top Edge", "Top-Right", "Right Edge", "Btm-Right",
    "Bottom", "Btm-Left", "Left Edge", "Top-Left"
};

/* ── Mutable quad copies ─────────────────────────────────────────── */

void editor_init_quads(editor_t *ed, level_data_t *level)
{
    i32 count = 0;
    for (i32 s = 0; s < level->sector_count; s++)
        count += level->sectors[s].quad_count;

    ed->edit_quads = (level_quad_t *)malloc(sizeof(level_quad_t) * count);
    ed->edit_quad_count = count;

    i32 idx = 0;
    for (i32 s = 0; s < level->sector_count; s++)
        for (i32 w = 0; w < level->sectors[s].quad_count; w++)
            ed->edit_quads[idx++] = *(level_quad_t *)&level->sectors[s].quads[w];
}

void editor_free_quads(editor_t *ed)
{
    if (ed->edit_quads) free(ed->edit_quads);
    ed->edit_quads = NULL;
    ed->edit_quad_count = 0;
}

static level_quad_t *get_edit_quad(editor_t *ed, i32 sector, i32 wall)
{
    i32 idx = 0;
    for (i32 s = 0; s < ed->level->sector_count; s++)
        for (i32 w = 0; w < ed->level->sectors[s].quad_count; w++)
            if (s == sector && w == wall) return &ed->edit_quads[idx];
            else idx++;
    return NULL;
}

/* ── Matrix helpers ──────────────────────────────────────────────── */

static void build_model_matrix(const level_quad_t *quad, f32 *out)
{
    f32 ry[16], rx[16], rz[16], tmp[16];
    mat4_rotate_y(ry, -DEG2RAD(quad->rot.y));
    mat4_rotate_x(rx, -DEG2RAD(quad->rot.x));
    mat4_rotate_z(rz, -DEG2RAD(quad->rot.z));
    mat4_multiply(tmp, ry, rx);
    mat4_multiply(out, tmp, rz);
    out[12] = quad->pos.x;
    out[13] = quad->pos.y + quad->size.y * 0.5f;
    out[14] = quad->pos.z;
}

static void transform_local(const f32 *m, vec3s lp, vec3s *out)
{
    out->x = m[0] * lp.x + m[4] * lp.y + m[8]  * lp.z + m[12];
    out->y = m[1] * lp.x + m[5] * lp.y + m[9]  * lp.z + m[13];
    out->z = m[2] * lp.x + m[6] * lp.y + m[10] * lp.z + m[14];
}

static vec3s local_point(const level_quad_t *quad, wall_point_t pt)
{
    f32 hx = quad->size.x * 0.5f;
    f32 hy = quad->size.y * 0.5f;
    switch (pt) {
        case POINT_TOP:            return (vec3s){ 0.0f,  hy, 0.0f};
        case POINT_TOP_RIGHT:      return (vec3s){  hx,  hy, 0.0f};
        case POINT_RIGHT:          return (vec3s){  hx, 0.0f, 0.0f};
        case POINT_BOTTOM_RIGHT:   return (vec3s){  hx, -hy, 0.0f};
        case POINT_BOTTOM:         return (vec3s){0.0f,  -hy, 0.0f};
        case POINT_BOTTOM_LEFT:    return (vec3s){ -hx, -hy, 0.0f};
        case POINT_LEFT:           return (vec3s){ -hx, 0.0f, 0.0f};
        case POINT_TOP_LEFT:       return (vec3s){ -hx,  hy, 0.0f};
        default:                   return (vec3s){0};
    }
}

/* ── GL draw helpers ─────────────────────────────────────────────── */

static void set_model_uniform(const f32 *m)
{
    GLint loc = glGetUniformLocation(state.data->program, "model");
    glUniformMatrix4fv(loc, 1, GL_FALSE, m);
}

static void draw_wireframe_quad(const level_quad_t *quad, const vec4s color)
{
    f32 model[16];
    build_model_matrix(quad, model);
    set_model_uniform(model);

    f32 hx = quad->size.x * 0.5f;
    f32 hy = quad->size.y * 0.5f;
    f32 verts[] = {
         hx,  hy, 0.0f,   color.x, color.y, color.z,
         hx, -hy, 0.0f,   color.x, color.y, color.z,
        -hx, -hy, 0.0f,   color.x, color.y, color.z,
        -hx,  hy, 0.0f,   color.x, color.y, color.z,
    };
    u32 inds[] = {0, 1, 1, 2, 2, 3, 3, 0};

    u32 vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void *)(3 * sizeof(f32)));
    glDisable(GL_DEPTH_TEST);
    glLineWidth(2.0f);
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
    glLineWidth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

static void draw_point_marker(const f32 *model, const vec4s color, f32 size)
{
    f32 h = size * 0.5f;
    f32 verts[] = {
         h,  h, 0.0f,   color.x, color.y, color.z,
         h, -h, 0.0f,   color.x, color.y, color.z,
        -h, -h, 0.0f,   color.x, color.y, color.z,
        -h,  h, 0.0f,   color.x, color.y, color.z,
    };
    u32 inds[] = {0, 1, 2, 0, 2, 3};

    u32 vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(inds), inds, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void *)(3 * sizeof(f32)));
    set_model_uniform(model);
    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

/* ── Render ──────────────────────────────────────────────────────── */

static void render_sector(const level_sector_data_t *sector)
{
    const vec4s c = {1.0f, 1.0f, 1.0f, 1.0f};
    for (i32 i = 0; i < sector->quad_count; i++)
        draw_wireframe_quad(&sector->quads[i], c);
}

void editor_render(const level_data_t *level)
{
    for (i32 i = 0; i < level->sector_count; i++)
        render_sector(&level->sectors[i]);
}

void editor_render_wireframe(const level_data_t *level, const editor_t *ed)
{
    for (i32 s = 0; s < level->sector_count; s++) {
        for (i32 w = 0; w < level->sectors[s].quad_count; w++) {
            const level_quad_t *quad = &level->sectors[s].quads[w];
            vec4s color = {1.0f, 1.0f, 1.0f, 1.0f};
            if (ed->mode >= EDITOR_WALL_SELECTED &&
                ed->selected_sector == s && ed->selected_wall == w)
                color = (vec4s){1.0f, 0.84f, 0.0f, 1.0f};
            draw_wireframe_quad(quad, color);
        }
    }

    if (ed->mode < EDITOR_WALL_SELECTED) return;

    const level_quad_t *quad = get_edit_quad((editor_t *)ed, ed->selected_sector, ed->selected_wall);
    if (!quad) return;

    f32 model[16];
    build_model_matrix(quad, model);

    vec4s edge_color   = {0.0f, 1.0f, 0.0f, 1.0f};
    vec4s corner_color = {1.0f, 1.0f, 0.0f, 1.0f};
    vec4s sel_color    = {1.0f, 0.0f, 0.0f, 1.0f};

    for (i32 i = 0; i < POINT_COUNT; i++) {
        vec3s lp = local_point(quad, (wall_point_t)i);
        bool is_corner = (i % 2 == 1);
        f32 sz = is_corner ? 0.3f : 0.2f;

        f32 point_model[16];
        memcpy(point_model, model, sizeof(point_model));
        point_model[12] = model[0] * lp.x + model[4] * lp.y + model[8]  * lp.z + model[12];
        point_model[13] = model[1] * lp.x + model[5] * lp.y + model[9]  * lp.z + model[13];
        point_model[14] = model[2] * lp.x + model[6] * lp.y + model[10] * lp.z + model[14];

        vec4s col = (i == ed->selected_point) ? sel_color
                  : is_corner ? corner_color : edge_color;
        draw_point_marker(point_model, col, sz);
    }
}

/* ── Input ───────────────────────────────────────────────────────── */

static bool is_corner(wall_point_t pt) { return pt % 2 == 1; }

void editor_handle_input(editor_t *ed, const camera_t *cam)
{
    (void)cam;

    bool cur[GLFW_KEY_LAST + 1];
    for (i32 i = 0; i <= GLFW_KEY_LAST; i++)
        cur[i] = glfwGetKey(state.win, i) == GLFW_PRESS;

    #define PRESSED(k) (cur[k] && !ed->_prev_keys[k])

    if (ed->mode == EDITOR_FREE) {
        if (PRESSED(GLFW_KEY_ENTER)) {
            editor_look_at_info_t info = editor_get_look_at_info(ed->level, cam, 50.0f);
            if (info.hit) {
                ed->mode = EDITOR_WALL_SELECTED;
                ed->selected_sector = info.sector_id;
                ed->selected_wall = info.wall_id;
                ed->selected_point = POINT_TOP;
                ed->move_speed = 0.1f;
            }
        }
    } else if (ed->mode == EDITOR_WALL_SELECTED) {
        if (PRESSED(GLFW_KEY_X)) ed->mode = EDITOR_FREE;
        else if (PRESSED(GLFW_KEY_ENTER)) ed->mode = EDITOR_EDITING_POINT;
        else if (PRESSED(GLFW_KEY_LEFT))
            ed->selected_point = (wall_point_t)((ed->selected_point + POINT_COUNT - 1) % POINT_COUNT);
        else if (PRESSED(GLFW_KEY_RIGHT))
            ed->selected_point = (wall_point_t)((ed->selected_point + 1) % POINT_COUNT);
    } else if (ed->mode == EDITOR_EDITING_POINT) {
        if (PRESSED(GLFW_KEY_X)) { ed->mode = EDITOR_WALL_SELECTED; goto done; }

        if (ed->selected_sector < 0 || ed->selected_sector >= ed->level->sector_count) {
            ed->mode = EDITOR_FREE; goto done;
        }
        if (ed->selected_wall < 0 || ed->selected_wall >= ed->level->sectors[ed->selected_sector].quad_count) {
            ed->mode = EDITOR_WALL_SELECTED; goto done;
        }

        level_quad_t *q = get_edit_quad(ed, ed->selected_sector, ed->selected_wall);
        if (!q) goto done;

        f32 dx = 0, dy = 0;
        if (PRESSED(GLFW_KEY_A)) dx -= ed->move_speed;
        if (PRESSED(GLFW_KEY_D)) dx += ed->move_speed;
        if (PRESSED(GLFW_KEY_W)) dy += ed->move_speed;
        if (PRESSED(GLFW_KEY_S)) dy -= ed->move_speed;

        if (dx != 0 || dy != 0) {
            wall_point_t pt = ed->selected_point;
            f32 nsx = q->size.x, nsy = q->size.y;
            f32 npx = q->pos.x, npy = q->pos.y;

            if (is_corner(pt)) {
                switch (pt) {
                    case POINT_TOP_RIGHT:    nsx += dx; npx += dx*0.5f; nsy += dy; npy += dy*0.5f;  break;
                    case POINT_BOTTOM_RIGHT: nsx += dx; npx += dx*0.5f; nsy -= dy; npy += dy;       break;
                    case POINT_BOTTOM_LEFT:  nsx -= dx; npx += dx*0.5f; nsy -= dy; npy += dy;       break;
                    case POINT_TOP_LEFT:     nsx -= dx; npx += dx*0.5f; nsy += dy; npy += dy*0.5f;  break;
                    default: break;
                }
            } else {
                switch (pt) {
                    case POINT_TOP:    nsy += dy; npy += dy*0.5f;  break;
                    case POINT_BOTTOM: nsy -= dy; npy += dy;        break;
                    case POINT_LEFT:   nsx -= dx; npx += dx*0.5f;  break;
                    case POINT_RIGHT:  nsx += dx; npx += dx*0.5f;  break;
                    default: break;
                }
            }
            if (nsx >= 0.1f && nsy >= 0.1f) {
                q->size.x = nsx; q->size.y = nsy;
                q->pos.x  = npx; q->pos.y  = npy;
            }
        }
    }
done:
    memcpy(ed->_prev_keys, cur, sizeof(cur));
    #undef PRESSED
}

/* ── Raycast ─────────────────────────────────────────────────────── */

static bool ray_intersects_quad(vec3s origin, vec3s dir, const level_quad_t *q, f32 *out_t, vec3s *out_hit)
{
    f32 m[16];
    build_model_matrix(q, m);

    vec3s wp = {q->pos.x, q->pos.y + q->size.y * 0.5f, q->pos.z};
    vec3s n = {m[8], m[9], m[10]};

    f32 denom = vec3_dot(n, dir);
    if (fabsf(denom) < 1e-4f) return false;

    f32 t = vec3_dot(vec3_sub(wp, origin), n) / denom;
    if (t < 0) return false;

    vec3s hit = vec3_add(origin, vec3_scale(dir, t));
    vec3s lh = vec3_sub(hit, wp);
    f32 hx = q->size.x * 0.5f, hy = q->size.y * 0.5f;

    if (fabsf(lh.x) <= hx + 0.01f && fabsf(lh.y) <= hy + 0.01f) {
        *out_t = t;
        *out_hit = hit;
        return true;
    }
    return false;
}

editor_look_at_info_t editor_get_look_at_info(const level_data_t *level, const camera_t *cam, f32 max_dist)
{
    editor_look_at_info_t info = {0};
    info.distance = max_dist;
    vec3s origin = cam->pos;
    vec3s dir = vec3_normalize(cam->front);
    f32 best = max_dist;

    for (i32 s = 0; s < level->sector_count; s++) {
        for (i32 w = 0; w < level->sectors[s].quad_count; w++) {
            f32 t; vec3s hit;
            if (ray_intersects_quad(origin, dir, &level->sectors[s].quads[w], &t, &hit) && t < best) {
                best = t;
                info.hit = true;
                info.sector_id = level->sectors[s].id;
                info.wall_id = w;
                info.distance = t;
                info.hit_position = hit;
                info.quad = &level->sectors[s].quads[w];
                info.sector = &level->sectors[s];
            }
        }
    }
    return info;
}

/* ── HUD ─────────────────────────────────────────────────────────── */

void editor_render_look_at_info(const level_data_t *level, const camera_t *cam, const editor_t *ed)
{
    f32 x = (f32)state.fb->w - 520.0f, y = 10.0f, lh = 20.0f;

    const char *modes[] = {"EDITOR: FREE", "EDITOR: WALL SEL", "EDITOR: EDIT POINT"};
    text_draw((vec2s){x, y}, modes[ed->mode]); y += lh;

    if (ed->mode == EDITOR_FREE) {
        editor_look_at_info_t info = editor_get_look_at_info(level, cam, 50.0f);
        if (info.hit) {
            text_draw((vec2s){x, y}, "LOOKING AT:"); y += lh;
            text_draw((vec2s){x, y}, "  Sector ID: %d", info.sector_id); y += lh;
            text_draw((vec2s){x, y}, "  Wall ID: %d", info.wall_id); y += lh;
            text_draw((vec2s){x, y}, "  Distance: %.2f", info.distance); y += lh;
            text_draw((vec2s){x, y}, "  Pos: %.2f %.2f %.2f", info.quad->pos.x, info.quad->pos.y, info.quad->pos.z); y += lh;
            text_draw((vec2s){x, y}, "  Rot: %.1f %.1f %.1f", info.quad->rot.x, info.quad->rot.y, info.quad->rot.z); y += lh;
            text_draw((vec2s){x, y}, "  Size: %.2f %.2f", info.quad->size.x, info.quad->size.y); y += lh;
            text_draw((vec2s){x, y}, "  Solid: %s", info.quad->is_solid ? "YES" : "NO"); y += lh;
            text_draw((vec2s){x, y}, "  Invisible: %s", info.quad->is_invisible ? "YES" : "NO"); y += lh;
            text_draw((vec2s){x, y}, "  Tex: %d", info.quad->tex_idx); y += lh;
            text_draw((vec2s){x, y}, "  Light: %.2f %.2f %.2f", info.sector->light.x, info.sector->light.y, info.sector->light.z); y += lh;
            text_draw((vec2s){x, y}, "  Color: %.2f %.2f %.2f", info.quad->color.x, info.quad->color.y, info.quad->color.z);
        } else text_draw((vec2s){x, y}, "LOOKING AT: Nothing");
    } else if (ed->mode == EDITOR_WALL_SELECTED) {
        text_draw((vec2s){x, y}, "Wall: Sector %d / Wall %d", ed->selected_sector, ed->selected_wall); y += lh;
        text_draw((vec2s){x, y}, "Point: %s", point_names[ed->selected_point]); y += lh;
        text_draw((vec2s){x, y}, "LEFT/RIGHT: cycle points"); y += lh;
        text_draw((vec2s){x, y}, "ENTER: edit point"); y += lh;
        text_draw((vec2s){x, y}, "X: deselect wall"); y += lh;
    } else {
        const level_quad_t *q = get_edit_quad((editor_t *)ed, ed->selected_sector, ed->selected_wall);
        if (!q) { text_draw((vec2s){x, y}, "ERROR: Invalid wall"); return; }
        text_draw((vec2s){x, y}, "Editing: %s", point_names[ed->selected_point]); y += lh;
        text_draw((vec2s){x, y}, "W/A/S/D: resize wall"); y += lh;
        text_draw((vec2s){x, y}, "X: back to point sel"); y += lh;
        text_draw((vec2s){x, y}, "Size: %.2f x %.2f", q->size.x, q->size.y);
    }
}
