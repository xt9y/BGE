#include "level.h"
#include "state.h"
#include <glad/glad.h>
#include "util/types.h"
#include "cam.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static u32 g_editor_vao = 0;
static u32 g_editor_vbo = 0;
static u32 g_editor_ebo = 0;
static bool g_editor_vao_initialized = false;

static void ensure_editor_vao(void)
{
    if (g_editor_vao_initialized) return;

    glGenVertexArrays(1, &g_editor_vao);
    glGenBuffers(1, &g_editor_vbo);
    glGenBuffers(1, &g_editor_ebo);

    glBindVertexArray(g_editor_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_editor_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_editor_ebo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    g_editor_vao_initialized = true;
}

void editor_add_sector(level_data_t* level) 
{
    i32 selected_sector_id = state.editor->selected_sector ? state.editor->selected_sector->id : -1;
    
    if (level->sector_capacity <= level->sector_count) {
        i32 new_cap = (level->sector_capacity == 0) ? level->sector_count + 16 : level->sector_capacity * 2;
        level_sector_data_t* new_sectors = malloc(sizeof(level_sector_data_t) * new_cap);
        if (level->sector_count > 0) memcpy(new_sectors, level->sectors, sizeof(level_sector_data_t) * level->sector_count);
        if (level->sector_capacity > 0) free(level->sectors);
        level->sectors = new_sectors;
        level->sector_capacity = new_cap;
        
        if (selected_sector_id != -1)
            for (i32 i = 0; i < level->sector_count; i++)
                if (level->sectors[i].id == selected_sector_id) { state.editor->selected_sector = &level->sectors[i]; break; }
    }
    
    level->sectors[level->sector_count].id = 0; 
    for (i32 i = 0; i < level->sector_count; i++)
        if (level->sectors[i].id >= level->sectors[level->sector_count].id) level->sectors[level->sector_count].id = level->sectors[i].id + 1;

    level->sectors[level->sector_count].light = (vec3s){1.0f, 1.0f, 1.0f};
    level->sectors[level->sector_count].quads = NULL;
    level->sectors[level->sector_count].quad_count = 0;
    level->sectors[level->sector_count].quad_capacity = 0;
    
    level->sector_count++;
}

void editor_add_quad(level_sector_data_t* sector, const level_quad_t* template) 
{
    if (sector->quad_capacity <= sector->quad_count) {
        i32 new_cap = (sector->quad_capacity == 0) ? sector->quad_count + 16 : sector->quad_capacity * 2;
        level_quad_t* new_quads = malloc(sizeof(level_quad_t) * new_cap);
        if (sector->quad_count > 0) memcpy(new_quads, sector->quads, sizeof(level_quad_t) * sector->quad_count);
        if (sector->quad_capacity > 0) free(sector->quads);
        sector->quads = new_quads;
        sector->quad_capacity = new_cap;
    }

    level_quad_t new_q = template ? *template : state.editor->template_quad; 
    new_q.sector_id = sector->id;

    sector->quads[sector->quad_count] = new_q;
    sector->quad_count++;
    state.editor->selected_quad = &sector->quads[sector->quad_count - 1];
    state.editor->selected_sector = sector;
    state.editor->selected_wall_idx = sector->quad_count - 1;
}

void editor_delete_quad(level_sector_data_t* sector, i32 idx) 
{
    if (!sector || idx < 0 || idx >= sector->quad_count) return;
    for (i32 i = idx; i < sector->quad_count - 1; i++) sector->quads[i] = sector->quads[i + 1];    
    sector->quad_count--;
}

int count_portal_quads(level_data_t* level, int portal_id) 
{
    if (portal_id <= 0) return 0;
    int count = 0;
    for (int s = 0; s < level->sector_count; s++)
        for (int i = 0; i < level->sectors[s].quad_count; i++)
            if (level->sectors[s].quads[i].portal_id == portal_id) count++;

    return count;
}

level_sector_data_t* get_sector_by_id(i32 id) 
{
    for (i32 i = 0; i < state.editor->level->sector_count; i++) if (state.editor->level->sectors[i].id == id) return &state.editor->level->sectors[i];
    return NULL;
}

void editor_move_quad_to_sector(level_sector_data_t* old_sector, level_sector_data_t* new_sector, i32 quad_idx) {
    if (!old_sector || !new_sector || quad_idx < 0 || quad_idx >= old_sector->quad_count) return;
    old_sector->quads[quad_idx].sector_id = new_sector->id;
    
    if (new_sector->quad_capacity <= new_sector->quad_count) {
        i32 new_cap = (new_sector->quad_capacity == 0) ? new_sector->quad_count + 16 : new_sector->quad_capacity * 2;
        level_quad_t* new_quads = malloc(sizeof(level_quad_t) * new_cap);
        if (new_sector->quad_count > 0) memcpy(new_quads, new_sector->quads, sizeof(level_quad_t) * new_sector->quad_count);
        if (new_sector->quad_capacity > 0) free(new_sector->quads);
        new_sector->quads = new_quads;
        new_sector->quad_capacity = new_cap;
    }
    
    new_sector->quads[new_sector->quad_count++] = old_sector->quads[quad_idx];
    
    for (i32 i = quad_idx; i < old_sector->quad_count - 1; i++) old_sector->quads[i] = old_sector->quads[i + 1];
    old_sector->quad_count--;
}

static vec3s intersect_ray_plane(vec3s ray_origin, vec3s ray_dir, vec3s plane_pos, vec3s plane_normal) {
    f32 denom = vec3_dot(plane_normal, ray_dir);
    if (fabsf(denom) < 0.0001f) return plane_pos;
    f32 t = vec3_dot(vec3_sub(plane_pos, ray_origin), plane_normal) / denom;
    return vec3_add(ray_origin, vec3_scale(ray_dir, t));
}

editor_look_at_info_t editor_get_look_at_info_with_ray(level_data_t* level, vec3s ray_origin, vec3s ray_dir, f32 max_dist) {
    editor_look_at_info_t best = {0};
    best.distance = max_dist;
    
    for (i32 s = 0; s < level->sector_count; s++) {
        level_sector_data_t* sector = &level->sectors[s];
        for (i32 i = 0; i < sector->quad_count; i++) {
            level_quad_t* quad = &sector->quads[i];
            f32 t; vec3s hit, local_hit;
            if (level_ray_intersects_quad(ray_origin, ray_dir, quad, &t, &hit, &local_hit)) {
                if (t < best.distance) {
                    best.hit = true;
                    best.distance = t;
                    best.wall_id = i;
                    best.sector_id = sector->id;
                    best.hit_position = hit;
                    best.local_hit = local_hit;
                    best.quad = quad;
                    best.sector = sector;
                }
            }
        }
    }
    return best;
}

void editor_update()
{
    static bool template_init = false;
    if (!template_init) {
        state.editor->template_quad = get_default_quad(state.cam);
        state.editor->template_mods = EDITOR_MOD_NONE;
        template_init = true;
    }

    if (!state.editor->selected_quad) {
        state.editor->template_quad.pos = (vec3s){
            roundf(state.cam->pos.x + state.cam->front.x * 3.0f),
            roundf(state.cam->pos.y + state.cam->front.y * 3.0f),
            roundf(state.cam->pos.z + state.cam->front.z * 3.0f)
        };
    }

    editor_look_at_info_t info = editor_get_look_at_info_with_ray(state.editor->level, state.cam->pos, vec3_normalize(state.cam->front), 100.0f);

    static bool mouse_was_pressed = false;
    bool mouse_is_pressed = glfwGetMouseButton(state.win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    bool ctrl_held = glfwGetKey(state.win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS;

    if (mouse_is_pressed && !mouse_was_pressed) 
    {
        if (state.editor->id == EDITOR_PAINT) 
        {
            if (info.hit) {
                if (state.editor->template_mods & EDITOR_MOD_COLOR) info.quad->color = state.editor->template_quad.color;
                if (state.editor->template_mods & EDITOR_MOD_ROTATION) info.quad->rot = state.editor->template_quad.rot;
                if (state.editor->template_mods & EDITOR_MOD_TEXTURE) info.quad->tex_id = state.editor->template_quad.tex_id;
                if (state.editor->template_mods & EDITOR_MOD_SECTOR) {
                    if (info.quad->sector_id != state.editor->template_quad.sector_id) {
                        level_sector_data_t* target_sector = get_sector_by_id(state.editor->template_quad.sector_id);
                        if (target_sector) editor_move_quad_to_sector(info.sector, target_sector, info.wall_id);
                    }
                } state.editor->selected_quad = NULL;
            } else {
                state.editor->selected_quad = NULL;
                state.editor->template_quad = get_default_quad(state.cam);
                state.editor->template_mods = EDITOR_MOD_NONE;
                state.editor->id = EDITOR_IDLE; 
            }
        } 
        else if (info.hit) 
        {
            state.editor->selected_quad = info.quad;
            state.editor->selected_sector = info.sector;
            state.editor->selected_wall_idx = info.wall_id;
            state.editor->template_quad = *info.quad;
            state.editor->template_mods = EDITOR_MOD_ALL;
            
            state.editor->drag_start_hit = info.hit_position;
            state.editor->drag_plane_normal = vec3_scale(state.cam->front, -1.0f);
            state.editor->drag_cam_start_pos = state.cam->pos;
            state.editor->drag_quad_start_pos = info.quad->pos;
            state.editor->drag_quad_start_rot = info.quad->rot;
            state.editor->drag_quad_start_size = info.quad->size;

            if (ctrl_held) {
                f32 dy_t = info.quad->size.y - info.local_hit.y;
                f32 dx_r = info.quad->size.x - info.local_hit.x;
                f32 tol = 5.0f;
                if (dy_t < dx_r && dy_t < tol) state.editor->id = EDITOR_RESIZE_TOP;
                else if (dx_r < tol) state.editor->id = EDITOR_RESIZE_RIGHT;
                else state.editor->id = EDITOR_DRAG;
            } else state.editor->id = EDITOR_DRAG;
        } else 
        {
            state.editor->selected_quad = NULL;
            state.editor->template_quad = get_default_quad(state.cam);
            state.editor->template_mods = EDITOR_MOD_NONE;
            state.editor->id = EDITOR_IDLE;
        }
    }

    if (!mouse_is_pressed) {
        if (state.editor->id != EDITOR_PAINT) state.editor->id = EDITOR_IDLE;
        state.editor->hover_id = EDITOR_IDLE;

        if (state.editor->selected_quad && ctrl_held) {
            if (info.hit && info.quad == state.editor->selected_quad) {
                f32 dy_t = info.quad->size.y - info.local_hit.y, dx_r = info.quad->size.x - info.local_hit.x, tol = 5.0f;
                if (dy_t < dx_r && dy_t < tol) state.editor->hover_id = EDITOR_RESIZE_TOP;
                else if (dx_r < tol) state.editor->hover_id = EDITOR_RESIZE_RIGHT;
            }
        }
    }

    if (state.editor->id != EDITOR_IDLE && state.editor->selected_quad) 
    {
        vec3s plane_pos = vec3_add(state.editor->drag_start_hit, vec3_sub(state.cam->pos, state.editor->drag_cam_start_pos));
        vec3s current_hit = intersect_ray_plane(state.cam->pos, vec3_normalize(state.cam->front), plane_pos, state.editor->drag_plane_normal);
        
        if (state.editor->id == EDITOR_DRAG) 
        {
            vec3s new_pos = vec3_add(state.editor->drag_quad_start_pos, vec3_sub(current_hit, state.editor->drag_start_hit));
            state.editor->selected_quad->pos = (vec3s){roundf(new_pos.x), roundf(new_pos.y), roundf(new_pos.z)};
        }
        else 
        {
            vec3s right = vec3_normalize(vec3_cross(state.cam->front, (vec3s){0, 1, 0}));
            vec3s up_vec = vec3_normalize(vec3_cross(right, state.cam->front));
            vec3s movement = vec3_add(vec3_scale(right, vec3_dot(vec3_sub(current_hit, plane_pos), right)), vec3_scale(up_vec, vec3_dot(vec3_sub(current_hit, plane_pos), up_vec)));

            f32 r_y[16], r_x[16], r_z[16], m[16], t[16];
            mat4_rotate_y(r_y, -DEG2RAD(state.editor->drag_quad_start_rot.y));
            mat4_rotate_x(r_x, -DEG2RAD(state.editor->drag_quad_start_rot.x));
            mat4_rotate_z(r_z, -DEG2RAD(state.editor->drag_quad_start_rot.z));
            mat4_multiply(t, r_y, r_x);
            mat4_multiply(m, t, r_z);

            vec3s local_right = { m[0], m[1], m[2] }, local_up = { m[4], m[5], m[6] };

            if (state.editor->id == EDITOR_RESIZE_TOP) state.editor->selected_quad->size.y = roundf(fmaxf(1.0f, state.editor->drag_quad_start_size.y + vec3_dot(movement, local_up)));
            if (state.editor->id == EDITOR_RESIZE_RIGHT) state.editor->selected_quad->size.x = roundf(fmaxf(1.0f, state.editor->drag_quad_start_size.x + vec3_dot(movement, local_right)));
            
            state.editor->selected_quad->pos = state.editor->drag_quad_start_pos;
        }
    }

    mouse_was_pressed = mouse_is_pressed;
}

static void render_border_segments(const level_quad_t* quad, const vec4s color, bool top, bool bottom, bool left, bool right)
{
    if (!quad) return;
    ensure_editor_vao();

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

    glUniformMatrix4fv(state.data->u_model, 1, GL_FALSE, model);
    texture_bind(texture_get_fallback(), 0);

    const f32 t = 0.08f;
    const f32 z = 0.01f;

    f32 vertices[16 * 8];
    u32 indices[4 * 6];
    u32 vcount = 0, icount = 0;

#define PUSH_QUAD(x0, y0, x1, y1) \
    do { \
        const u32 base = vcount / 8; \
        vertices[vcount++] = (x1); vertices[vcount++] = (y1); vertices[vcount++] = z; vertices[vcount++] = color.x; vertices[vcount++] = color.y; vertices[vcount++] = color.z; vertices[vcount++] = 0; vertices[vcount++] = 0; \
        vertices[vcount++] = (x1); vertices[vcount++] = (y0); vertices[vcount++] = z; vertices[vcount++] = color.x; vertices[vcount++] = color.y; vertices[vcount++] = color.z; vertices[vcount++] = 0; vertices[vcount++] = 0; \
        vertices[vcount++] = (x0); vertices[vcount++] = (y0); vertices[vcount++] = z; vertices[vcount++] = color.x; vertices[vcount++] = color.y; vertices[vcount++] = color.z; vertices[vcount++] = 0; vertices[vcount++] = 0; \
        vertices[vcount++] = (x0); vertices[vcount++] = (y1); vertices[vcount++] = z; vertices[vcount++] = color.x; vertices[vcount++] = color.y; vertices[vcount++] = color.z; vertices[vcount++] = 0; vertices[vcount++] = 0; \
        indices[icount++] = base + 0; indices[icount++] = base + 1; indices[icount++] = base + 3; \
        indices[icount++] = base + 1; indices[icount++] = base + 2; indices[icount++] = base + 3; \
    } while (0)

    if (top) PUSH_QUAD(0.0f, quad->size.y - t, quad->size.x, quad->size.y);
    if (bottom) PUSH_QUAD(0.0f, 0.0f, quad->size.x, t);
    if (left) PUSH_QUAD(0.0f, 0.0f, t, quad->size.y);
    if (right) PUSH_QUAD(quad->size.x - t, 0.0f, quad->size.x, quad->size.y);

#undef PUSH_QUAD

    glBindVertexArray(g_editor_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_editor_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(f32) * vcount, vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_editor_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u32) * icount, indices, GL_DYNAMIC_DRAW);
    glDrawElements(GL_TRIANGLES, (GLsizei)icount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

static void editor_render_sector(const level_sector_data_t *sector)
{
    for (i32 i = 0; i < sector->quad_count; i++) {
        if (state.editor->selected_quad == &sector->quads[i]) {
            editor_e border_id = state.editor->id != EDITOR_IDLE ? state.editor->id : state.editor->hover_id;
            bool top_yellow = true, bottom_yellow = true, left_yellow = true, right_yellow = true;

            if (border_id == EDITOR_RESIZE_TOP) top_yellow = false;
            if (border_id == EDITOR_RESIZE_RIGHT) right_yellow = false;

            const vec4s select_color = { 1.0f, 1.0f, 0.0f, 1.0f };
            render_border_segments(&sector->quads[i], select_color, top_yellow, bottom_yellow, left_yellow, right_yellow);

            const vec4s resize_color = { 1.0f, 0.0f, 1.0f, 1.0f };
            if (border_id == EDITOR_RESIZE_TOP) render_border_segments(&sector->quads[i], resize_color, true, false, false, false);
            if (border_id == EDITOR_RESIZE_RIGHT) render_border_segments(&sector->quads[i], resize_color, false, false, false, true);
        }
    }
}

void editor_render_info();
void editor_render_legend();

void editor_render()
{
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_DEPTH_TEST);
    editor_render_info();
    editor_render_legend();
    for (i32 i = 0; i < state.editor->level->sector_count; i++) editor_render_sector(&state.editor->level->sectors[i]);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
}

void editor_render_info()
{
    f32 x = 10.0f, y = 150.0f, line_height = 20.0f;

    level_quad_t* q = state.editor->selected_quad;
    level_sector_data_t* s = state.editor->selected_sector;
    bool is_template = false;

    if (!q) { q = &state.editor->template_quad; s = NULL; is_template = true; }

    text_draw((vec2s){x, y}, is_template ? "TEMPLATE (BRUSH):" : "SELECTED:"); y += line_height;
    
    char mod_s = (is_template && (state.editor->template_mods & EDITOR_MOD_SECTOR)) ? '*' : ' ';
    char mod_t = (is_template && (state.editor->template_mods & EDITOR_MOD_TEXTURE)) ? '*' : ' ';
    char mod_c = (is_template && (state.editor->template_mods & EDITOR_MOD_COLOR)) ? '*' : ' ';
    char mod_r = (is_template && (state.editor->template_mods & EDITOR_MOD_ROTATION)) ? '*' : ' ';

    text_draw((vec2s){x, y}, "%c Sector ID: %d", mod_s, is_template ? q->sector_id : s->id); y += line_height;
    if (!is_template) { text_draw((vec2s){x, y}, "  Wall ID: %d", state.editor->selected_wall_idx); y += line_height; }
    if (!is_template) { text_draw((vec2s){x, y}, "  Pos: %.0f %.0f %.0f", q->pos.x, q->pos.y, q->pos.z); y += line_height; }
    text_draw((vec2s){x, y}, "%c Rot: %.0f %.0f %.0f", mod_r, q->rot.x, q->rot.y, q->rot.z); y += line_height;
    if (!is_template) { text_draw((vec2s){x, y}, "  Size: %.0f %.0f", q->size.x, q->size.y); y += line_height; }
    text_draw((vec2s){x, y}, "%c Tex ID: %d", mod_t, q->tex_id); y += line_height;
    if (!is_template) { text_draw((vec2s){x, y}, "  Portal ID: %d", q->portal_id); y += line_height; }
    if (!is_template) { text_draw((vec2s){x, y}, "  Portal Side Flip: %s", q->portal_side_flip ? "ON" : "OFF"); y += line_height; }
    if (!is_template && s) { text_draw((vec2s){x, y}, "  Light: %.1f %.1f %.1f", s->light.x, s->light.y, s->light.z); y += line_height; }
    text_draw((vec2s){x, y}, "%c Color: %.1f %.1f %.1f", mod_c, q->color.x, q->color.y, q->color.z);
}

void editor_render_legend()
{
    f32 x = 10.0f, y = (f32)state.fb->h - 30.0f, line_height = 20.0f;
    text_draw((vec2s){x, y}, "  SHFT+B:BIL   I:SLD     SHFT+I:INV   P:+PRTLV        SHIFT+P:-PRTL  CTRL+P:PRTL_SIDE"); y -= line_height;
    text_draw((vec2s){x, y}, "  1-3:+CLR     4-6:+LIT  7-9:+ROT     SHFT+7-9:-ROT   0:TEX_ID       Q:+SEC            SHFT+Q:-SEC"); y -= line_height;
    text_draw((vec2s){x, y}, "  N:NEW        X:DEL     R:RESET      ENTER:DESEL     DRAG:MOVE      CTRL:RESIZE       V:PAINT_MODE"); y -= line_height;
    text_draw((vec2s){x, y}, "QUAD SETTINGS:"); y -= line_height;
    text_draw((vec2s){x, y}, "  ESC:EXIT     E:PLAY    TAB:CURS     B:NEXT_LVL"); y -= line_height;
    text_draw((vec2s){x, y}, "SETTINGS: "); y -= line_height; 
    text_draw((vec2s){x, y}, "EDITOR INPUT INFO:");
}

void editor_save(level_data_t* level)
{
    level->cam.pos = state.cam->pos;
    level->cam.yaw = state.cam->yaw;
    level->cam.pitch = state.cam->pitch;

    char full_path[256];
    sprintf(full_path, "Engine/res/%s", level->path);
    FILE* f = fopen(full_path, "w");
    if (!f) return;

    int level_num = 0;
    sscanf(level->name, "Level %d", &level_num);
    if (level_num == 0) sscanf(level->path, "level%d.h", &level_num);

    fprintf(f, "#ifndef LEVEL%d_H\n", level_num);
    fprintf(f, "#define LEVEL%d_H\n\n", level_num);
    fprintf(f, "#include \"level.h\"\n\n");

    for (int s = 0; s < level->sector_count; s++) {
        level_sector_data_t* sector = &level->sectors[s];
        if (sector->quad_count > 0) {
            fprintf(f, "static level_quad_t level%d_sector%d_quads[] = {\n", level_num, s);
            for (int q = 0; q < sector->quad_count; q++) {
                level_quad_t* quad = &sector->quads[q];
                fprintf(f, "    { .pos = {%.0f, %.0f, %.0f}, .rot = {%.0f, %.0f, %.0f}, .size = {%.0f, %.0f}, .tex_id = %d, .is_solid = %s, .is_invisible = %s, .is_billboard = %s, .portal_side_flip = %s, .color = {%.1ff, %.1ff, %.1ff}, .portal_id = %d, .sector_id = %d },\n",
                    roundf(quad->pos.x), roundf(quad->pos.y), roundf(quad->pos.z),
                    roundf(quad->rot.x), roundf(quad->rot.y), roundf(quad->rot.z),
                    roundf(quad->size.x), roundf(quad->size.y),
                    quad->tex_id,
                    quad->is_solid ? "true" : "false",
                    quad->is_invisible ? "true" : "false",
                    quad->is_billboard ? "true" : "false",
                    quad->portal_side_flip ? "true" : "false",
                    quad->color.x, quad->color.y, quad->color.z,
                    quad->portal_id,
                    quad->sector_id);
            }
            fprintf(f, "};\n\n");
        }
    }

    fprintf(f, "static level_sector_data_t level%d_sectors[] = {\n", level_num);
    for (int s = 0; s < level->sector_count; s++) {
        level_sector_data_t* sector = &level->sectors[s];
        if (sector->quad_count == 0) continue;
        fprintf(f, "    { .id = %d, .light = {%.1ff, %.1ff, %.1ff}, .quads = level%d_sector%d_quads, .quad_count = sizeof(level%d_sector%d_quads) / sizeof(level%d_sector%d_quads[0]) },\n", 
            sector->id, sector->light.x, sector->light.y, sector->light.z, level_num, s, level_num, s, level_num, s);
    }
    fprintf(f, "};\n\n");

    fprintf(f, "static inline level_data_t load_%d(void)\n{\n", level_num);
    fprintf(f, "    return (level_data_t){\n");
    fprintf(f, "        .name = \"%s\",\n", level->name);
    fprintf(f, "        .path = \"%s\",\n", level->path);
    fprintf(f, "        .sectors = level%d_sectors,\n", level_num);
    fprintf(f, "        .sector_count = sizeof(level%d_sectors) / sizeof(level%d_sectors[0]),\n", level_num, level_num);
    fprintf(f, "        .cam = { .pos = {%.3ff, %.3ff, %.3ff}, .yaw = %.3ff, .pitch = %.3ff }\n", 
        level->cam.pos.x, level->cam.pos.y, level->cam.pos.z,
        level->cam.yaw, level->cam.pitch);
    fprintf(f, "    };\n}\n\n");

    fprintf(f, "#endif\n");
    fclose(f);
}
