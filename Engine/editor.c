#include "level.h"
#include "state.h"
#include <glad/glad.h>
#include "util/types.h"
#include "cam.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static void editor_add_quad(level_sector_data_t* sector, const level_quad_t* template) {
    level_quad_t* new_quads = malloc(sizeof(level_quad_t) * (sector->quad_count + 1));
    if (sector->quad_count > 0) {
        memcpy(new_quads, sector->quads, sizeof(level_quad_t) * sector->quad_count);
    }
    level_quad_t new_q;
    if (template) {
        new_q = *template;
        new_q.pos.y += 0.5f; // Offset duplication
    } else {
        new_q = (level_quad_t){ 
            .pos = vec3_add(state.cam->pos, vec3_scale(state.cam->front, 3.0f)), 
            .rot = {0,0,0}, .size = {2,2}, .tex_idx = 0, .color = {1,1,1}, .is_solid = true 
        };
    }
    new_quads[sector->quad_count] = new_q;
    sector->quads = new_quads;
    sector->quad_count++;
    state.editor->selected_quad = &sector->quads[sector->quad_count - 1];
    state.editor->selected_sector = sector;
    state.editor->selected_wall_idx = sector->quad_count - 1;
}

static void editor_delete_quad(level_sector_data_t* sector, i32 idx) {
    if (!sector || idx < 0 || idx >= sector->quad_count) return;
    if (sector->quad_count == 1) {
        sector->quad_count = 0;
        return;
    }
    level_quad_t* new_quads = malloc(sizeof(level_quad_t) * (sector->quad_count - 1));
    if (idx > 0) memcpy(new_quads, sector->quads, sizeof(level_quad_t) * idx);
    if (idx < sector->quad_count - 1) memcpy(new_quads + idx, sector->quads + idx + 1, sizeof(level_quad_t) * (sector->quad_count - idx - 1));
    sector->quads = new_quads;
    sector->quad_count--;
}

static void render_wall_quad_outline(const level_quad_t* quad, const vec4s color)
{
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

    f32 half_x = 0.5f * quad->size.x + 0.02f;
    f32 half_y = 0.5f * quad->size.y + 0.02f;

    f32 vertices[] = {
         half_x,  half_y, 0.0f,   color.x, color.y, color.z,
         half_x, -half_y, 0.0f,   color.x, color.y, color.z,
        -half_x, -half_y, 0.0f,   color.x, color.y, color.z,
        -half_x,  half_y, 0.0f,   color.x, color.y, color.z,
    };

    u32 indices[] = {
        0, 1, 1, 2, 2, 3, 3, 0
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)(3 * sizeof(f32)));

    glLineWidth(8.0f);
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);

    f32 offset = 0.08f;
    f32 vertices2[] = {
         half_x + offset,  half_y + offset, 0.0f,   color.x, color.y, color.z,
         half_x + offset, -half_y - offset, 0.0f,   color.x, color.y, color.z,
        -half_x - offset, -half_y - offset, 0.0f,   color.x, color.y, color.z,
        -half_x - offset,  half_y + offset, 0.0f,   color.x, color.y, color.z,
    };

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices2), vertices2);
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);

    glLineWidth(1.0f);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

static vec3s intersect_ray_plane(vec3s origin, vec3s dir, vec3s plane_pos, vec3s plane_normal) {
    f32 denom = vec3_dot(plane_normal, dir);
    if (fabsf(denom) < 0.0001f) return plane_pos;
    f32 t = vec3_dot(vec3_sub(plane_pos, origin), plane_normal) / denom;
    return vec3_add(origin, vec3_scale(dir, t));
}

static bool ray_intersects_quad(const vec3s ray_origin, const vec3s ray_dir, const level_quad_t* quad, f32* out_t, vec3s* out_hit)
{
    f32 rot_y[16], rot_x[16], rot_z[16], model[16], temp[16];
    mat4_rotate_y(rot_y, -DEG2RAD(quad->rot.y));
    mat4_rotate_x(rot_x, -DEG2RAD(quad->rot.x));
    mat4_rotate_z(rot_z, -DEG2RAD(quad->rot.z));
    mat4_multiply(temp, rot_y, rot_x);
    mat4_multiply(model, temp, rot_z);

    vec3s wall_pos = {
        quad->pos.x, quad->pos.y + quad->size.y * 0.5f, quad->pos.z
    };

    vec3s normal = {
        model[0] * 0.0f + model[4] * 0.0f + model[8] * 1.0f,
        model[1] * 0.0f + model[5] * 0.0f + model[9] * 1.0f,
        model[2] * 0.0f + model[6] * 0.0f + model[10] * 1.0f
    };

    f32 denom = vec3_dot(normal, ray_dir);
    if (fabsf(denom) < 0.0001f) return false;

    f32 t = vec3_dot(vec3_sub(wall_pos, ray_origin), normal) / denom;
    if (t < 0.0f) return false;

    vec3s hit = vec3_add(ray_origin, vec3_scale(ray_dir, t));
    vec3s local_hit = vec3_sub(hit, wall_pos);

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

    f32 half_x = 0.5f * quad->size.x, half_y = 0.5f * quad->size.y;

    if (fabsf(unrotated_hit.x) <= half_x + 0.01f && fabsf(unrotated_hit.y) <= half_y + 0.01f) {
        *out_t = t;
        *out_hit = hit;
        return true;
    }
    return false;
}

static editor_look_at_info_t editor_get_look_at_info_with_ray(const level_data_t* level, vec3s ray_origin, vec3s ray_dir, f32 max_distance)
{
    editor_look_at_info_t info = {0};
    info.distance = max_distance;
    f32 closest_t = max_distance;

    for (i32 s = 0; s < level->sector_count; s++)
    {
        for (i32 w = 0; w < level->sectors[s].quad_count; w++)
        {
            f32 t; vec3s hit;
            if (ray_intersects_quad(ray_origin, ray_dir, &level->sectors[s].quads[w], &t, &hit) && t < closest_t) {
                closest_t = t;
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

void editor_update()
{
    vec3s ray_dir = vec3_normalize(state.cam->front);
    editor_look_at_info_t info = editor_get_look_at_info_with_ray(state.editor->level, state.cam->pos, ray_dir, 100.0f);

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
        } else {
            state.editor->selected_quad = NULL;
        }
    }

    if (!mouse_is_pressed) state.editor->is_dragging = false;


    if (state.editor->is_dragging && state.editor->selected_quad) {
        vec3s cam_move = vec3_sub(state.cam->pos, state.editor->drag_cam_start_pos);
        vec3s plane_pos = vec3_add(state.editor->drag_start_hit, cam_move);
        vec3s current_hit = intersect_ray_plane(state.cam->pos, ray_dir, plane_pos, state.editor->drag_plane_normal);
        
        vec3s diff = vec3_sub(current_hit, state.editor->drag_start_hit);
        vec3s rel_diff = vec3_sub(current_hit, plane_pos);

        vec3s right = vec3_normalize(vec3_cross(state.cam->front, (vec3s){0, 1, 0}));
        vec3s up_vec = vec3_normalize(vec3_cross(right, state.cam->front));
        f32 dx = vec3_dot(rel_diff, right);
        f32 dy = vec3_dot(rel_diff, up_vec);

        if (glfwGetKey(state.win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
            state.editor->selected_quad->size.x = fmaxf(0.1f, state.editor->drag_quad_start_size.x + dx);
            state.editor->selected_quad->size.y = fmaxf(0.1f, state.editor->drag_quad_start_size.y + dy);
        } else state.editor->selected_quad->pos = vec3_add(state.editor->drag_quad_start_pos, diff);
    }

    bool shift_held = glfwGetKey(state.win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(state.win, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

    static f32 tex_timer = 0;
    if (state.editor->selected_quad) {
        bool next_tex = glfwGetKey(state.win, GLFW_KEY_0) == GLFW_PRESS && !shift_held;
        bool prev_tex = glfwGetKey(state.win, GLFW_KEY_0) == GLFW_PRESS && shift_held;

        if (next_tex || prev_tex) {
            if (tex_timer <= 0) {
                if (next_tex) state.editor->selected_quad->tex_idx++;
                else state.editor->selected_quad->tex_idx--;
                
                if (state.editor->selected_quad->tex_idx >= state.text->count) state.editor->selected_quad->tex_idx = -1;
                else if (state.editor->selected_quad->tex_idx < -1) state.editor->selected_quad->tex_idx = state.text->count - 1;
                
                tex_timer = 0.15f;
            }
            tex_timer -= state.dt;
        } else tex_timer = 0;
    }

    mouse_was_pressed = mouse_is_pressed;

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

    // Discrete/Continuous editing with fixed steps
    static f32 adj_timers[9] = {0};
    static const int adj_keys[] = { GLFW_KEY_1, GLFW_KEY_2, GLFW_KEY_3, GLFW_KEY_4, GLFW_KEY_5, GLFW_KEY_6, GLFW_KEY_7, GLFW_KEY_8, GLFW_KEY_9 };
    for (int i = 0; i < 9; i++) {
        if (glfwGetKey(state.win, adj_keys[i]) == GLFW_PRESS) {
            if (adj_timers[i] <= 0) {
                f32 step;
                if (i < 6) step = 0.01f;
                else step = 1.0f;
                if (shift_held) step = -step;

                if (i < 3 && state.editor->selected_quad) {
                    if (i == 0) state.editor->selected_quad->color.x = fmaxf(0, fminf(1, state.editor->selected_quad->color.x + step));
                    if (i == 1) state.editor->selected_quad->color.y = fmaxf(0, fminf(1, state.editor->selected_quad->color.y + step));
                    if (i == 2) state.editor->selected_quad->color.z = fmaxf(0, fminf(1, state.editor->selected_quad->color.z + step));
                } else if (i < 6 && state.editor->selected_sector) {
                    if (i == 3) state.editor->selected_sector->light.x = fmaxf(0, fminf(1, state.editor->selected_sector->light.x + step));
                    if (i == 4) state.editor->selected_sector->light.y = fmaxf(0, fminf(1, state.editor->selected_sector->light.y + step));
                    if (i == 5) state.editor->selected_sector->light.z = fmaxf(0, fminf(1, state.editor->selected_sector->light.z + step));
                } else if (state.editor->selected_quad) {
                    if (i == 6) state.editor->selected_quad->rot.x += step;
                    if (i == 7) state.editor->selected_quad->rot.y += step;
                    if (i == 8) state.editor->selected_quad->rot.z += step;
                }
                adj_timers[i] = 0.02f;
            }
            adj_timers[i] -= state.dt;
        } else adj_timers[i] = 0;
    }

    static bool r_pressed = false;
    if (glfwGetKey(state.win, GLFW_KEY_R) == GLFW_PRESS) {
        if (!r_pressed && state.editor->selected_quad) {
            state.editor->selected_quad->rot = (vec3s){0, 0, 0};
            state.editor->selected_quad->size = (vec2s){2, 2};
            state.editor->selected_quad->tex_idx = 0;
            state.editor->selected_quad->color = (vec3s){1, 1, 1};
            state.editor->selected_quad->is_solid = true;
            state.editor->selected_quad->is_invisible = false;
            r_pressed = true;
        }
    } else r_pressed = false;
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
            fprintf(f, "    { .pos = {%.3ff, %.3ff, %.3ff}, .rot = {%.3ff, %.3ff, %.3ff}, .size = {%.3ff, %.3ff}, .tex_idx = %d, .is_solid = %s, .is_invisible = %s, .color = {%.3ff, %.3ff, %.3ff} },\n",
                quad->pos.x, quad->pos.y, quad->pos.z,
                quad->rot.x, quad->rot.y, quad->rot.z,
                quad->size.x, quad->size.y,
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
        fprintf(f, "    { .id = %d, .light = {%.3ff, %.3ff, %.3ff}, .quads = level%d_sector%d_quads, ", sector->id, sector->light.x, sector->light.y, sector->light.z, level_num, s);
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

void editor_render()
{
    vec3s ray_dir = vec3_normalize(state.cam->front);
    editor_look_at_info_t info = editor_get_look_at_info_with_ray(state.editor->level, state.cam->pos, ray_dir, 100.0f);
    
    if (info.hit) render_wall_quad_outline(info.quad, (vec4s){1.0f, 1.0f, 1.0f, 1.0f});
}

void editor_render_look_at_info()
{
    vec3s ray_dir = vec3_normalize(state.cam->front);
    editor_look_at_info_t info = editor_get_look_at_info_with_ray(state.editor->level, state.cam->pos, ray_dir, 100.0f);
    
    level_quad_t* quad = info.hit ? info.quad : state.editor->selected_quad;
    level_sector_data_t* sector = info.hit ? info.sector : state.editor->selected_sector;
    i32 wall_id = info.hit ? info.wall_id : state.editor->selected_wall_idx;

    f32 x = 10.0f, y = 150.0f, line_height = 20.0f;

    if (quad && sector) {
        text_draw((vec2s){x, y}, info.hit ? "LOOKING AT:" : "SELECTED:"); y += line_height;
        text_draw((vec2s){x, y}, "  Sector ID: %d", sector->id); y += line_height;
        text_draw((vec2s){x, y}, "  Wall ID: %d", wall_id); y += line_height;
        if (info.hit) { text_draw((vec2s){x, y}, "  Distance: %.3f", info.distance); y += line_height; }
        text_draw((vec2s){x, y}, "  Pos: %.1f %.1f %.1f", quad->pos.x, quad->pos.y, quad->pos.z); y += line_height;
        text_draw((vec2s){x, y}, "  Rot: %.1f %.1f %.1f", quad->rot.x, quad->rot.y, quad->rot.z); y += line_height;
        text_draw((vec2s){x, y}, "  Size: %.1f %.1f", quad->size.x, quad->size.y); y += line_height;
        text_draw((vec2s){x, y}, "  Solid: %s", quad->is_solid ? "YES" : "NO"); y += line_height;
        text_draw((vec2s){x, y}, "  Invisible: %s", quad->is_invisible ? "YES" : "NO"); y += line_height;
        text_draw((vec2s){x, y}, "  Tex: %d", quad->tex_idx); y += line_height;
        text_draw((vec2s){x, y}, "  Light: %.1f %.1f %.1f", sector->light.x, sector->light.y, sector->light.z); y += line_height;
        text_draw((vec2s){x, y}, "  Color: %.1f %.1f %.1f", quad->color.x, quad->color.y, quad->color.z);
    } else text_draw((vec2s){x, y}, "LOOKING AT: Nothing");

    text_draw((vec2s){(f32)state.fb->w * 0.5f - 5.0f, (f32)state.fb->h * 0.5f - 10.0f}, "+");
}

void editor_render_legend(void)
{
    f32 x = 10.0f, y = (f32)state.fb->h - 30.0f, line_height = 20.0f;
    text_draw((vec2s){x, y}, "ESC:EXIT E:PLAY TAB:CURS CLICK:DRAG CTRL:RESIZE B:NEXT N:NEW X:DEL R:RESET"); y -= line_height;
    text_draw((vec2s){x, y}, "1-3:CLR 4-6:LIT 7-9:ROT 0:TEX_ID");
}

