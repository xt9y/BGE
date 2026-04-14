#include "level.h"
#include "state.h"
#include <glad/glad.h>
#include "util/types.h"
#include "cam.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static level_quad_t get_default_quad() 
{
    return (level_quad_t) { 
        .pos = {roundf(state.cam->pos.x + state.cam->front.x * 3.0f), roundf(state.cam->pos.y + state.cam->front.y * 3.0f), roundf(state.cam->pos.z + state.cam->front.z * 3.0f)}, 
        .rot = {0, 0, 0}, .size = {2, 2}, .tex_idx = 0, .color = {1, 1, 1}, .is_solid = true, .is_invisible = false
    };
}

static void editor_add_quad(level_sector_data_t* sector, const level_quad_t* template) 
{
    if (sector->quad_capacity <= sector->quad_count) {
        i32 new_cap = (sector->quad_capacity == 0) ? sector->quad_count + 64 : sector->quad_capacity * 2;
        level_quad_t* new_quads = malloc(sizeof(level_quad_t) * new_cap);
        if (sector->quad_count > 0) memcpy(new_quads, sector->quads, sizeof(level_quad_t) * sector->quad_count);
        if (sector->quad_capacity > 0) free(sector->quads);
        sector->quads = new_quads;
        sector->quad_capacity = new_cap;
    }

    level_quad_t new_q = template ? *template : get_default_quad(); 

    sector->quads[sector->quad_count] = new_q;
    sector->quad_count++;
    state.editor->selected_quad = &sector->quads[sector->quad_count - 1];
    state.editor->selected_sector = sector;
    state.editor->selected_wall_idx = sector->quad_count - 1;
}

static void editor_delete_quad(level_sector_data_t* sector, i32 idx) 
{
    if (!sector || idx < 0 || idx >= sector->quad_count) return;

    for (i32 i = idx; i < sector->quad_count - 1; i++) sector->quads[i] = sector->quads[i + 1];    
    sector->quad_count--;
}

static void render_wall_quad(const level_quad_t* quad, const vec4s color)
{
    if (!quad) return;

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

    f32 hx = quad->size.x,
        hy = quad->size.y,
        t = 0.05f, // thickness 
        z = 0.0f;

    f32 vertices[] = {
        hx,     hy,     z,  color.x, color.y, color.z, 0, 0,
        hx,     hy - t, z,  color.x, color.y, color.z, 0, 0,
        0,      hy - t, z,  color.x, color.y, color.z, 0, 0,
        0,      hy,     z,  color.x, color.y, color.z, 0, 0,
        hx,     t,      z,  color.x, color.y, color.z, 0, 0,
        hx,     0,      z,  color.x, color.y, color.z, 0, 0,
        0,      0,      z,  color.x, color.y, color.z, 0, 0,
        0,      t,      z,  color.x, color.y, color.z, 0, 0,
        t,      hy,     z,  color.x, color.y, color.z, 0, 0,
        t,      0,      z,  color.x, color.y, color.z, 0, 0,
        0,      0,      z,  color.x, color.y, color.z, 0, 0,
        0,      hy,     z,  color.x, color.y, color.z, 0, 0,
        hx,     hy,     z,  color.x, color.y, color.z, 0, 0,
        hx,     0,      z,  color.x, color.y, color.z, 0, 0,
        hx - t, 0,      z,  color.x, color.y, color.z, 0, 0,
        hx - t, hy,     z,  color.x, color.y, color.z, 0, 0,
    };

    u32 indices[] = {
        0, 1, 3, 1, 2, 3, 
        4, 5, 7, 5, 6, 7,
        8, 9, 11, 9, 10, 11,
        12, 13, 15, 13, 14, 15
    };

    u32 vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));

    texture_bind(texture_get_by_name("solid_color"), 0);
    texture_bind(texture_get_by_name("solid_color"), 1);

    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

static vec3s intersect_ray_plane(vec3s origin, vec3s dir, vec3s plane_pos, vec3s plane_normal) 
{
    if (fabsf(vec3_dot(plane_normal, dir)) < 0.0001f) return plane_pos;
    return vec3_add(origin, vec3_scale(dir, vec3_dot(vec3_sub(plane_pos, origin), plane_normal) / vec3_dot(plane_normal, dir)));
}

static editor_look_at_info_t editor_get_look_at_info_with_ray(const level_data_t* level, vec3s ray_origin, vec3s ray_dir, f32 max_distance)
{
    editor_look_at_info_t info = {0};
    info.distance = max_distance;
    f32 closest_t = max_distance;

    for (i32 s = 0; s < level->sector_count; s++) {
        for (i32 w = 0; w < level->sectors[s].quad_count; w++) {
            f32 t; vec3s hit, lhit;
            if (level_ray_intersects_quad(ray_origin, ray_dir, &level->sectors[s].quads[w], &t, &hit, &lhit) && t < closest_t) {
                closest_t = t;
                info.hit = true;
                info.sector_id = level->sectors[s].id;
                info.wall_id = w;
                info.distance = t;
                info.hit_position = hit;
                info.local_hit = lhit;
                info.quad = &level->sectors[s].quads[w];
                info.sector = &level->sectors[s];
            }
        }
    }
    return info;
}

void editor_update()
{
    editor_look_at_info_t info = editor_get_look_at_info_with_ray(state.editor->level, state.cam->pos, vec3_normalize(state.cam->front), 100.0f);

    static bool mouse_was_pressed = false;
    bool mouse_is_pressed = glfwGetMouseButton(state.win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    if (mouse_is_pressed && !mouse_was_pressed) {
        if (info.hit) {
            state.editor->selected_quad = info.quad;
            state.editor->selected_sector = info.sector;
            state.editor->selected_wall_idx = info.wall_id;
            state.editor->is_dragging = true;
            state.editor->drag_start_hit = info.hit_position;
            state.editor->drag_plane_normal = vec3_scale(state.cam->front, -1.0f);
            state.editor->drag_cam_start_pos = state.cam->pos;
            state.editor->drag_quad_start_pos = info.quad->pos;
            state.editor->drag_quad_start_rot = info.quad->rot;
            state.editor->drag_quad_start_size = info.quad->size;

            f32 dy_t = info.quad->size.y - info.local_hit.y;
            f32 dx_r = info.quad->size.x - info.local_hit.x;
            
            f32 tol = 5.0f;
            if (dy_t < dx_r && dy_t < tol) state.editor->drag_edge = EDGE_TOP;
            else if (dx_r < tol) state.editor->drag_edge = EDGE_RIGHT;
            else state.editor->drag_edge = EDGE_NONE;
        } else {
            state.editor->selected_quad = NULL;
        }
    }

    if (!mouse_is_pressed) {
        state.editor->is_dragging = false;
        state.editor->hover_edge = EDGE_NONE;

        if (state.editor->selected_quad && (glfwGetKey(state.win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)) {
            if (info.hit && info.quad == state.editor->selected_quad) {
                f32 dy_t = info.quad->size.y - info.local_hit.y,
                    dx_r = info.quad->size.x - info.local_hit.x,
                    tol = 5.0f;

                if (dy_t < dx_r && dy_t < tol) state.editor->hover_edge = EDGE_TOP;
                else if (dx_r < tol) state.editor->hover_edge = EDGE_RIGHT;
            }
        }
    }

    bool shift_held = glfwGetKey(state.win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;


    if (state.editor->is_dragging && state.editor->selected_quad) {
        vec3s plane_pos = vec3_add(state.editor->drag_start_hit, vec3_sub(state.cam->pos, state.editor->drag_cam_start_pos));
        vec3s current_hit = intersect_ray_plane(state.cam->pos, vec3_normalize(state.cam->front), plane_pos, state.editor->drag_plane_normal);
        
        vec3s right = vec3_normalize(vec3_cross(state.cam->front, (vec3s){0, 1, 0}));
        vec3s up_vec = vec3_normalize(vec3_cross(right, state.cam->front));

        if (glfwGetKey(state.win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
            vec3s movement = vec3_add(vec3_scale(right, vec3_dot(vec3_sub(current_hit, plane_pos), right)), vec3_scale(up_vec, vec3_dot(vec3_sub(current_hit, plane_pos), up_vec)));

            f32 r_y[16], r_x[16], r_z[16], m[16], t[16];
            mat4_rotate_y(r_y, -DEG2RAD(state.editor->drag_quad_start_rot.y));
            mat4_rotate_x(r_x, -DEG2RAD(state.editor->drag_quad_start_rot.x));
            mat4_rotate_z(r_z, -DEG2RAD(state.editor->drag_quad_start_rot.z));
            mat4_multiply(t, r_y, r_x);
            mat4_multiply(m, t, r_z);

            vec3s local_right = { m[0], m[1], m[2] },
                  local_up = { m[4], m[5], m[6] };

            f32 d_size_x = 0, d_size_y = 0;

            if (state.editor->drag_edge == EDGE_TOP) d_size_y = vec3_dot(movement, local_up);
            else if (state.editor->drag_edge == EDGE_RIGHT) d_size_x = vec3_dot(movement, local_right);

            state.editor->selected_quad->size.x = roundf(fmaxf(1.0f, state.editor->drag_quad_start_size.x + d_size_x));
            state.editor->selected_quad->size.y = roundf(fmaxf(1.0f, state.editor->drag_quad_start_size.y + d_size_y));
            state.editor->selected_quad->pos = state.editor->drag_quad_start_pos;
        } else {
            vec3s new_pos = vec3_add(state.editor->drag_quad_start_pos, vec3_sub(current_hit, state.editor->drag_start_hit));
            state.editor->selected_quad->pos = (vec3s){roundf(new_pos.x), roundf(new_pos.y), roundf(new_pos.z)};
        }
    }

    static f32 tex_timer = 0;
    if (state.editor->selected_quad) {
        if (glfwGetKey(state.win, GLFW_KEY_0) == GLFW_PRESS) {
            if (tex_timer <= 0) {
                state.editor->selected_quad->tex_idx++;
                if (state.editor->selected_quad->tex_idx >= state.text->count) state.editor->selected_quad->tex_idx = -1;
                tex_timer = 0.15f;
            }
            tex_timer -= state.dt;
        } else tex_timer = 0;
    }

    static bool n_pressed = false;
    if (glfwGetKey(state.win, GLFW_KEY_N) == GLFW_PRESS) {
        if (!n_pressed) {
            level_sector_data_t* s = state.editor->selected_sector ? state.editor->selected_sector : &state.editor->level->sectors[0];
            editor_add_quad(s, state.editor->selected_quad);
            n_pressed = true;
        }
    } else n_pressed = false;

    static bool x_pressed = false;
    if (glfwGetKey(state.win, GLFW_KEY_X) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_DELETE) == GLFW_PRESS) {
        if (!x_pressed && state.editor->selected_quad) {
            editor_delete_quad(state.editor->selected_sector, state.editor->selected_wall_idx);
            state.editor->selected_quad = NULL;
            x_pressed = true;
        }
    } else x_pressed = false;

    static bool r_pressed = false;
    if (glfwGetKey(state.win, GLFW_KEY_R) == GLFW_PRESS) {
        if (!r_pressed && state.editor->selected_quad) {
            *state.editor->selected_quad = get_default_quad();
            r_pressed = true;
        }
    } else r_pressed = false;

    static bool i_pressed = false;
    if (glfwGetKey(state.win, GLFW_KEY_I) == GLFW_PRESS) {
        if (!i_pressed && state.editor->selected_quad) {
            if (shift_held) state.editor->selected_quad->is_invisible = !state.editor->selected_quad->is_invisible;
            else state.editor->selected_quad->is_solid = !state.editor->selected_quad->is_solid;
            i_pressed = true;
        }
    } else i_pressed = false;

    static bool adj_pressed[9] = {0};
    static f32 adj_timer[9] = {0};
    static const int adj_keys[] = { GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9 };
    for (int i = 0; i < 9; i++) {
        bool triggered = false;
        if (glfwGetKey(state.win, adj_keys[i]) == GLFW_PRESS) {
            if (!adj_pressed[i]) { triggered = true; adj_pressed[i] = true; adj_timer[i] = 0.3f; }
            if (adj_pressed[i] && i >= 6) { adj_timer[i] -= state.dt; if (adj_timer[i] <= 0) { triggered = true; adj_timer[i] = 0.05f; } }
        }
        if (glfwGetKey(state.win, adj_keys[i]) == GLFW_RELEASE) adj_pressed[i] = false;

        if (triggered) {
            f32* val = NULL;

            if (state.editor->selected_quad && i == 0) val = &state.editor->selected_quad->color.x;
            if (state.editor->selected_quad && i == 1) val = &state.editor->selected_quad->color.y;
            if (state.editor->selected_quad && i == 2) val = &state.editor->selected_quad->color.z;
            if (state.editor->selected_sector && i == 3) val = &state.editor->selected_sector->light.x;
            if (state.editor->selected_sector && i == 4) val = &state.editor->selected_sector->light.y;
            if (state.editor->selected_sector && i == 5) val = &state.editor->selected_sector->light.z;
            if (state.editor->selected_quad && i == 6) val = &state.editor->selected_quad->rot.x;
            if (state.editor->selected_quad && i == 7) val = &state.editor->selected_quad->rot.y;
            if (state.editor->selected_quad && i == 8) val = &state.editor->selected_quad->rot.z;

            if (val) {
                f32 step = (i < 6) ? 0.1f : 1.0f;
                if (i >= 6 && shift_held) step = -step;
                *val += step;
                if (i < 6 && *val > 1.05f)  *val = 0.0f;
                if (i < 6 && *val < -0.05f) *val = 1.0f;
                if (i < 6) *val = roundf(*val * 10.0f) / 10.0f;
                if (i >= 6 && *val >= 360.0f) *val = 0.0f;
                if (i >= 6 && *val < 0.0f)    *val = 359.0f;
                if (i >= 6) *val = roundf(*val);
            }
        }
    } 
    
    mouse_was_pressed = mouse_is_pressed;
}

void editor_save(level_data_t* level)
{
    level->cam_pos = state.cam->pos;
    level->cam_yaw = state.cam->yaw;
    level->cam_pitch = state.cam->pitch;

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
        fprintf(f, "static level_quad_t level%d_sector%d_quads[] = {\n", level_num, s);
        for (int q = 0; q < sector->quad_count; q++) {
            level_quad_t* quad = &sector->quads[q];
            fprintf(f, "    { .pos = {%.0f, %.0f, %.0f}, .rot = {%.0f, %.0f, %.0f}, .size = {%.0f, %.0f}, .tex_idx = %d, .is_solid = %s, .is_invisible = %s, .color = {%.1ff, %.1ff, %.1ff} },\n",
                roundf(quad->pos.x), roundf(quad->pos.y), roundf(quad->pos.z),
                roundf(quad->rot.x), roundf(quad->rot.y), roundf(quad->rot.z),
                roundf(quad->size.x), roundf(quad->size.y),
                quad->tex_idx,
                quad->is_solid ? "true" : "false",
                quad->is_invisible ? "true" : "false",
                quad->color.x, quad->color.y, quad->color.z);
        }
        fprintf(f, "};\n\n");
    }

    fprintf(f, "static level_sector_data_t level%d_sectors[] = {\n", level_num);
    for (int s = 0; s < level->sector_count; s++) {
        level_sector_data_t* sector = &level->sectors[s];
        fprintf(f, "    { .id = %d, .light = {%.1ff, %.1ff, %.1ff}, .quads = level%d_sector%d_quads, ", sector->id, sector->light.x, sector->light.y, sector->light.z, level_num, s);
        fprintf(f, ".quad_count = sizeof(level%d_sector%d_quads) / sizeof(level%d_sector%d_quads[0]) },\n", level_num, s, level_num, s);
    }
    fprintf(f, "};\n\n");

    fprintf(f, "static inline level_data_t load_%d(void)\n{\n", level_num);
    fprintf(f, "    return (level_data_t){\n");
    fprintf(f, "        .name = \"%s\",\n", level->name);
    fprintf(f, "        .path = \"%s\",\n", level->path);
    fprintf(f, "        .sectors = level%d_sectors,\n", level_num);
    fprintf(f, "        .sector_count = sizeof(level%d_sectors) / sizeof(level%d_sectors[0]),\n", level_num, level_num);
    fprintf(f, "        .cam_pos = {%.3ff, %.3ff, %.3ff},\n", state.cam->pos.x, state.cam->pos.y, state.cam->pos.z);
    fprintf(f, "        .cam_yaw = %.3ff,\n", state.cam->yaw);
    fprintf(f, "        .cam_pitch = %.3ff\n", state.cam->pitch);
    fprintf(f, "    };\n}\n\n");
    fprintf(f, "#endif\n");

    fclose(f);
}

static void render_resize_markers(const level_quad_t* quad, editor_edge_t edge, vec3s color)
{
    if (!quad || edge == EDGE_NONE) return;

    f32 hx = quad->size.x;
    f32 hy = quad->size.y;
    
    f32 r_y[16], r_x[16], r_z[16], model[16], t_mat[16];
    mat4_rotate_y(r_y, -DEG2RAD(quad->rot.y));
    mat4_rotate_x(r_x, -DEG2RAD(quad->rot.x));
    mat4_rotate_z(r_z, -DEG2RAD(quad->rot.z));
    mat4_multiply(t_mat, r_y, r_x);
    mat4_multiply(model, t_mat, r_z);

    model[12] = quad->pos.x;
    model[13] = quad->pos.y;
    model[14] = quad->pos.z;

    GLint model_loc = glGetUniformLocation(state.data->program, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);

    f32 t = 0.05f; // thickness
    f32 z = 0.0f;
    f32 v[32]; // 4 vertices * (3 pos + 3 color + 2 uv)

    if (edge == EDGE_TOP) {
        f32 data[] = {
             hx,     hy,     z,  color.x, color.y, color.z,  0,0,
             hx,     hy - t, z,  color.x, color.y, color.z,  0,0,
             0,      hy - t, z,  color.x, color.y, color.z,  0,0,
             0,      hy,     z,  color.x, color.y, color.z,  0,0,
        };
        memcpy(v, data, sizeof(data));
    } else if (edge == EDGE_RIGHT) {
        f32 data[] = {
             hx,     hy,     z,  color.x, color.y, color.z,  0,0,
             hx,     0,      z,  color.x, color.y, color.z,  0,0,
             hx - t, 0,      z,  color.x, color.y, color.z,  0,0,
             hx - t, hy,     z,  color.x, color.y, color.z,  0,0,
        };
        memcpy(v, data, sizeof(data));
    } else return;

    u32 indices[] = { 
        0, 1, 3, 1, 2, 3 
    };

    u32 vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STREAM_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));

    texture_bind(texture_get_by_name("solid_color"), 0);
    texture_bind(texture_get_by_name("solid_color"), 1);

    glDisable(GL_DEPTH_TEST);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

void editor_render_selected_info();
void editor_render_legend();

void editor_render()
{
    if (state.editor->selected_quad) render_wall_quad(state.editor->selected_quad, (vec4s){1.0f, 1.0f, 0.0f, 1.0f});

    if (glfwGetKey(state.win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
        if (state.editor->selected_quad) {
            editor_edge_t active_edge = state.editor->is_dragging ? state.editor->drag_edge : state.editor->hover_edge;
            render_resize_markers(state.editor->selected_quad, EDGE_TOP, active_edge == EDGE_TOP ? (vec3s){1.0f, 0.6f, 1.0f} : (vec3s){1.0f, 0.0f, 1.0f});
            render_resize_markers(state.editor->selected_quad, EDGE_RIGHT, active_edge == EDGE_RIGHT ? (vec3s){1.0f, 0.6f, 1.0f} : (vec3s){1.0f, 0.0f, 1.0f});
        }
    }

    editor_render_selected_info();
    editor_render_legend();
}

void editor_render_selected_info()
{
    f32 x = 10.0f, y = 150.0f, line_height = 20.0f;

    if (state.editor->selected_quad && state.editor->selected_sector) {
        text_draw((vec2s){x, y}, "SELECTED:"); y += line_height;
        text_draw((vec2s){x, y}, "  Sector ID: %d", state.editor->selected_sector->id); y += line_height;
        text_draw((vec2s){x, y}, "  Wall ID: %d", state.editor->selected_wall_idx); y += line_height;
        text_draw((vec2s){x, y}, "  Pos: %.0f %.0f %.0f", state.editor->selected_quad->pos.x, state.editor->selected_quad->pos.y, state.editor->selected_quad->pos.z); y += line_height;
        text_draw((vec2s){x, y}, "  Rot: %.0f %.0f %.0f", state.editor->selected_quad->rot.x, state.editor->selected_quad->rot.y, state.editor->selected_quad->rot.z); y += line_height;
        text_draw((vec2s){x, y}, "  Size: %.0f %.0f", state.editor->selected_quad->size.x, state.editor->selected_quad->size.y); y += line_height;
        text_draw((vec2s){x, y}, "  Solid: %s", state.editor->selected_quad->is_solid ? "YES" : "NO"); y += line_height;
        text_draw((vec2s){x, y}, "  Invisible: %s", state.editor->selected_quad->is_invisible ? "YES" : "NO"); y += line_height;
        text_draw((vec2s){x, y}, "  Tex: %d", state.editor->selected_quad->tex_idx); y += line_height;
        text_draw((vec2s){x, y}, "  Light: %.1f %.1f %.1f", state.editor->selected_sector->light.x, state.editor->selected_sector->light.y, state.editor->selected_sector->light.z); y += line_height;
        text_draw((vec2s){x, y}, "  Color: %.1f %.1f %.1f", state.editor->selected_quad->color.x, state.editor->selected_quad->color.y, state.editor->selected_quad->color.z);
    } else text_draw((vec2s){x, y}, "SELECTED: Nothing");
}

void editor_render_legend(void)
{
    f32 x = 10.0f, y = (f32)state.fb->h - 30.0f, line_height = 20.0f;
    text_draw((vec2s){x, y}, "ESC:EXIT E:PLAY TAB:CURS CLICK:DRAG CTRL:RESIZE B:NEXT N:NEW X:DEL R:RESET I:SLD SHFT+I:INV"); y -= line_height;
    text_draw((vec2s){x, y}, "1-3:+CLR 4-6:+LIT 7-9:+ROT SHFT+7-9:-ROT 0:TEX_ID");
}

