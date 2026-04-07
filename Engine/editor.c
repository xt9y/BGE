#include "level.h"
#include "state.h"
#include "gfx.h"
#include "util/types.h"
#include "cam.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <string.h>
#include <math.h>
#include <stdio.h>

static void render_wall_wireframe(const level_quad_t* quad, const vec4s color)
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

    f32 half_x = 0.5f * quad->size.x;
    f32 half_y = 0.5f * quad->size.y;

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(f32), (void*)(3 * sizeof(f32)));

    // glDisable(GL_DEPTH_TEST);
    glLineWidth(2.0f);
    glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
    glLineWidth(4.0f);
    glEnable(GL_DEPTH_TEST);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
}

static void render_sector(const level_sector_data_t *sector)
{
    for (i32 i = 0; i < sector->quad_count; i++)
    {
        const vec4s wall_color = {
            1.0f,
            1.0f,
            1.0f,
            1.0f
        };

        render_wall_wireframe(&sector->quads[i], wall_color);
    }
}

void editor_render(const level_data_t *level)
{
    for (i32 i = 0; i < level->sector_count; i++) render_sector(&level->sectors[i]);
}

typedef struct {
    bool hit;
    i32 sector_id;
    i32 wall_id;
    f32 distance;
    vec3s hit_position;
    const level_quad_t* quad;
    const level_sector_data_t* sector;
} look_at_info_t;

static bool ray_intersects_quad(const vec3s ray_origin, const vec3s ray_dir, const level_quad_t* quad, f32* out_t, vec3s* out_hit)
{
    f32 rot_y[16], rot_x[16], rot_z[16];
    mat4_rotate_y(rot_y, -DEG2RAD(quad->rot.y));
    mat4_rotate_x(rot_x, -DEG2RAD(quad->rot.x));
    mat4_rotate_z(rot_z, -DEG2RAD(quad->rot.z));

    f32 temp[16], model[16];
    mat4_multiply(temp, rot_y, rot_x);
    mat4_multiply(model, temp, rot_z);

    vec3s wall_pos = {quad->pos.x, quad->pos.y + quad->size.y * 0.5f, quad->pos.z};
    f32 half_x = 0.5f * quad->size.x;
    f32 half_y = 0.5f * quad->size.y;

    vec3s normal = {
        0.0f, 0.0f, 1.0f
    };

    vec3s rotated_normal = {
        model[0] * normal.x + model[4] * normal.y + model[8] * normal.z,
        model[1] * normal.x + model[5] * normal.y + model[9] * normal.z,
        model[2] * normal.x + model[6] * normal.y + model[10] * normal.z
    };

    f32 denom = vec3_dot(rotated_normal, ray_dir);
    if (fabsf(denom) < 0.0001f) return false; 

    f32 t = vec3_dot(vec3_sub(wall_pos, ray_origin), rotated_normal) / denom;
    if (t < 0.0f) return false;

    vec3s hit = vec3_add(ray_origin, vec3_scale(ray_dir, t));

    vec3s local_hit = vec3_sub(hit, wall_pos);
    
    f32 tolerance = 0.01f;
    if (fabsf(local_hit.x) <= half_x + tolerance && 
        fabsf(local_hit.y) <= half_y + tolerance) 
    {
        *out_t = t;
        *out_hit = hit;
        return true;
    }

    return false;
}

static look_at_info_t get_look_at_info(const level_data_t* level, const camera_t* cam, f32 max_distance)
{
    look_at_info_t info = {0};
    info.hit = false;
    info.distance = max_distance;

    vec3s ray_origin = cam->pos;
    vec3s ray_dir = vec3_normalize(cam->front);

    f32 closest_t = max_distance;
    vec3s closest_hit;

    for (i32 s = 0; s < level->sector_count; s++) 
    {
        const level_sector_data_t* sector = &level->sectors[s];
        
        for (i32 w = 0; w < sector->quad_count; w++) 
        {
            const level_quad_t* quad = &sector->quads[w];
            f32 t;
            vec3s hit;

            if (ray_intersects_quad(ray_origin, ray_dir, quad, &t, &hit)) 
            {
                if (t < closest_t) {
                    closest_t = t;
                    closest_hit = hit;
                    info.hit = true;
                    info.sector_id = sector->id;
                    info.wall_id = w;
                    info.distance = t;
                    info.hit_position = hit;
                    info.quad = quad;
                    info.sector = sector;
                }
            }
        }
    }

    return info;
}

void editor_render_look_at_info(const level_data_t* level, const camera_t* cam)
{
    look_at_info_t info = get_look_at_info(level, cam, 50.0f);

    f32 x = (f32)state.fb->w - 520.0f;
    f32 y = 10.0f;
    f32 line_height = 20.0f;

    if (info.hit) 
    {
        text_draw((vec2s){x, y}, "LOOKING AT:"); y += line_height;
        text_draw((vec2s){x, y}, "  Sector ID: %d", info.sector_id); y += line_height;
        text_draw((vec2s){x, y}, "  Wall ID: %d", info.wall_id); y += line_height;
        text_draw((vec2s){x, y}, "  Distance: %.2f", info.distance); y += line_height;
        text_draw((vec2s){x, y}, "  Pos: %.2f %.2f %.2f", info.quad->pos.x, info.quad->pos.y, info.quad->pos.z); y += line_height;
        text_draw((vec2s){x, y}, "  Rot: %.1f %.1f %.1f", info.quad->rot.x, info.quad->rot.y, info.quad->rot.z); y += line_height;
        text_draw((vec2s){x, y}, "  Size: %.2f %.2f", info.quad->size.x, info.quad->size.y); y += line_height;
        text_draw((vec2s){x, y}, "  Solid: %s", info.quad->is_solid ? "YES" : "NO"); y += line_height;
        text_draw((vec2s){x, y}, "  Invisible: %s", info.quad->is_invisible ? "YES" : "NO"); y += line_height;
        text_draw((vec2s){x, y}, "  Tex: %d", info.quad->tex_idx); y += line_height;
        text_draw((vec2s){x, y}, "  Light: %.2f %.2f %.2f", info.sector->light.x, info.sector->light.y, info.sector->light.z); y += line_height;
        text_draw((vec2s){x, y}, "  Color: %.2f %.2f %.2f", info.quad->color.x, info.quad->color.y, info.quad->color.z);
    } 

    else text_draw((vec2s){x, y}, "LOOKING AT: Nothing");
}

