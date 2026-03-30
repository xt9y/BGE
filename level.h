#ifndef LEVEL_H
#define LEVEL_H

#include "Engine/util/types.h"
#include "Engine/util/math.h"
#include "Engine/text.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_WALL_VERTICES 4096
#define MAX_WALLS_PER_SECTOR 256
#define MAX_SECTORS_PER_LEVEL 64

typedef struct
{
    i32 id;
    vec3s pos_start;
    vec3s pos_end;
    vec3s color;
    bool is_solid;
    bool is_invisible;
    i32 tex_idx;
} wall_t;

typedef struct
{
    i32 id;
    f32 light_intensity;
    wall_t *walls;
    u32 wall_count;
    f32 floor_height, ceil_height;
} sector_t;

typedef struct
{
    const char* name;
    const char* path;
    sector_t *sectors;
    u32 sector_count;
} level_t;

// ============================================================================
// LEVEL DATA STRUCTURES
// ============================================================================

// Quad definition - same as primitive_create_quad parameters
typedef struct
{
    vec3s pos;
    vec3s rot;
    vec2s size;
    i32 tex_idx;
    bool is_solid;
    bool is_invisible;
    vec3s color;
} level_quad_t;

typedef struct
{
    i32 id;
    f32 light_intensity;
    f32 floor_height;
    f32 ceil_height;
    const level_quad_t* quads;
    i32 quad_count;
} level_sector_data_t;

typedef struct
{
    const char* name;
    const char* path;
    const level_sector_data_t* sectors;
    i32 sector_count;
} level_data_t;

static inline wall_t wall_from_quad(const level_quad_t* quad, f32 floor_h, f32 ceil_h)
{
    f32 rad = DEG2RAD(quad->rot.y);
    f32 cos_r = cosf(rad);
    f32 sin_r = sinf(rad);
    
    f32 half_len = quad->size.x * 0.5f;
    vec3s start = {
        quad->pos.x - half_len * cos_r,
        quad->pos.y,
        quad->pos.z - half_len * sin_r
    };
    vec3s end = {
        quad->pos.x + half_len * cos_r,
        quad->pos.y,
        quad->pos.z + half_len * sin_r
    };

    wall_t wall = {
        .id = 0,
        .pos_start = start,
        .pos_end = end,
        .color = quad->color,
        .is_solid = quad->is_solid,
        .is_invisible = quad->is_invisible,
        .tex_idx = quad->tex_idx
    };
    return wall;
}

level_t level_load(const level_data_t* data);

// ============================================================================
// LEVEL RENDERING IMPLEMENTATION
// ============================================================================
#ifdef  LEVEL_RENDERING
#define LEVEL_RENDERING

#include "Engine/state.h"

void level_cleanup(level_t *level)
{
    if (level->sectors)
    {
        for (u32 i = 0; i < level->sector_count; i++)
        {
            if (level->sectors[i].walls)
            {
                free(level->sectors[i].walls);
                level->sectors[i].walls = NULL;
            }
        }
        free(level->sectors);
        level->sectors = NULL;
    }
    level->sector_count = 0;
}

static bool point_in_polygon(const vec3s pos, const wall_t *walls, const u32 wall_count)
{
    i32 crossings = 0;
    for (u32 i = 0; i < wall_count; i++)
    {
        const wall_t *w = &walls[i];
        if (((w->pos_start.z <= pos.z) && (w->pos_end.z > pos.z)) || 
            ((w->pos_end.z <= pos.z) && (w->pos_start.z > pos.z)))
        {
            const f32 vt = (pos.z - w->pos_start.z) / (w->pos_end.z - w->pos_start.z);
            if (pos.x < w->pos_start.x + vt * (w->pos_end.x - w->pos_start.x))
                crossings++;
        }
    }
    return (crossings % 2) == 1;
}

sector_t* level_find_player_sector(const level_t *level, const vec3s pos)
{
    for (u32 i = 0; i < level->sector_count; i++)
    {
        sector_t *sector = &level->sectors[i];
        if (point_in_polygon(pos, sector->walls, sector->wall_count))
            return sector;
    }
    return NULL;
}

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

static void render_wall_quad(const wall_t *wall, f32 bottom, f32 top, const vec4s color, i32 tex_idx)
{
    ensure_wall_vao();
    
    vec3s center = {
        (wall->pos_start.x + wall->pos_end.x) * 0.5f,
        (bottom + top) * 0.5f,
        (wall->pos_start.z + wall->pos_end.z) * 0.5f
    };
    
    f32 length = sqrtf(
        (wall->pos_end.x - wall->pos_start.x) * (wall->pos_end.x - wall->pos_start.x) +
        (wall->pos_end.z - wall->pos_start.z) * (wall->pos_end.z - wall->pos_start.z)
    );
    
    f32 height = top - bottom;
    
    // Calculate rotation from wall direction
    f32 dx = wall->pos_end.x - wall->pos_start.x;
    f32 dz = wall->pos_end.z - wall->pos_start.z;
    f32 angle = atan2f(dx, dz) * 180.0f / PI;
    
    f32 rot_y_mat[16], model[16];
    mat4_rotate_y(rot_y_mat, DEG2RAD(angle));
    memcpy(model, rot_y_mat, sizeof(model));
    model[12] = center.x;
    model[13] = center.y;
    model[14] = center.z;
    
    GLint model_loc = glGetUniformLocation(state.data->program, "model");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);

    // Bind texture (-1 = fallback texture from text.c)
    if (tex_idx >= 0 && tex_idx < state.text->count) {
        texture_bind(&state.text->textures[tex_idx], 0);
    } else {
        texture_bind(texture_get_fallback(), 0);
    }
    
    f32 half_x = 0.5f * length;
    f32 half_y = 0.5f * height;
    f32 u_repeat = length * 0.5f, v_repeat = height * 0.5f;
    
    f32 vertices[] = {
         half_x,  half_y, 0.0f,   color.x, color.y, color.z,   u_repeat, v_repeat,
         half_x, -half_y, 0.0f,   color.x, color.y, color.z,   u_repeat, 0.0f,
        -half_x, -half_y, 0.0f,   color.x, color.y, color.z,   0.0f,     0.0f,
        -half_x,  half_y, 0.0f,   color.x, color.y, color.z,   0.0f,     v_repeat,
    };
    
    u32 indices[] = { 0, 1, 3, 1, 2, 3 };
    
    glBindBuffer(GL_ARRAY_BUFFER, g_wall_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_wall_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glBindVertexArray(g_wall_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

#define points_match(ax, az, bx, bz) (fabsf(ax - bx) < 0.001f && fabsf(az - bz) < 0.001f)

static const sector_t* find_adjacent_sector(const level_t *level, const sector_t *current, const wall_t *wall)
{
    for (u32 si = 0; si < level->sector_count; ++si)
    {
        const sector_t *s = &level->sectors[si];
        if (s == current) continue;
        for (u32 wi = 0; wi < s->wall_count; ++wi)
        {
            const wall_t *w = &s->walls[wi];
            if ((points_match(wall->pos_start.x, wall->pos_start.z, w->pos_start.x, w->pos_start.z) && 
                 points_match(wall->pos_end.x, wall->pos_end.z, w->pos_end.x, w->pos_end.z)) ||
                (points_match(wall->pos_start.x, wall->pos_start.z, w->pos_end.x, w->pos_end.z) && 
                 points_match(wall->pos_end.x, wall->pos_end.z, w->pos_start.x, w->pos_start.z)))
                return s;
        }
    }
    return NULL;
}

static void render_sector(const level_t *level, const sector_t *sector)
{
    for (u32 i = 0; i < sector->wall_count; i++)
    {
        const wall_t *wall = &sector->walls[i];
        
        const vec4s wall_color = {
            wall->color.x * sector->light_intensity,
            wall->color.y * sector->light_intensity,
            wall->color.z * sector->light_intensity,
            1.0f
        };
        
        if (!wall->is_invisible)
        {
            render_wall_quad(wall, sector->floor_height, sector->ceil_height, wall_color, wall->tex_idx);
        }
        else
        {
            const sector_t *adj = find_adjacent_sector(level, sector, wall);
            if (adj)
            {
                const f32 eps = 0.0001f;
                const f32 f_bottom = fminf(sector->floor_height, adj->floor_height);
                const f32 f_top    = fmaxf(sector->floor_height, adj->floor_height);
                if (f_top - f_bottom > eps)
                {
                    render_wall_quad(wall, f_bottom, f_top, wall_color, wall->tex_idx);
                }
                
                const f32 c_bottom = fminf(sector->ceil_height, adj->ceil_height);
                const f32 c_top    = fmaxf(sector->ceil_height, adj->ceil_height);
                if (c_top - c_bottom > eps)
                {
                    render_wall_quad(wall, c_bottom, c_top, wall_color, wall->tex_idx);
                }
            }
        }
    }
}

void level_render(const level_t *level)
{
    for (u32 i = 0; i < level->sector_count; i++)
    {
        render_sector(level, &level->sectors[i]);
    }
}

static bool line_segment_intersect(const vec3s p1, const vec3s p2,
                                   const vec3s p3, const vec3s p4)
{
    const f32 denom = (p1.x - p2.x) * (p3.z - p4.z) - (p1.z - p2.z) * (p3.x - p4.x);
    if (fabsf(denom) < 0.0001f) return false;

    const f32 t = ((p1.x - p3.x) * (p3.z - p4.z) - (p1.z - p3.z) * (p3.x - p4.x)) / denom;
    const f32 u = -((p1.x - p2.x) * (p1.z - p3.z) - (p1.z - p2.z) * (p1.x - p3.x)) / denom;

    return (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f);
}

static void handle_collision(const wall_t *wall, vec3s *pos, const vec3s old_pos)
{
    if (!wall->is_solid) return;
    pos->x = old_pos.x;
    pos->z = old_pos.z;
}

bool level_check_collision(const level_t *level, vec3s *pos, const vec3s old_pos)
{
    bool collided = false;

    const sector_t *old_sector = level_find_player_sector(level, old_pos);
    const sector_t *new_sector = level_find_player_sector(level, *pos);

    if (old_sector)
    {
        for (u32 i = 0; i < old_sector->wall_count; i++)
        {
            const wall_t *wall = &old_sector->walls[i];
            if (!wall->is_solid) continue;
            if (line_segment_intersect(old_pos, *pos, wall->pos_start, wall->pos_end))
            {
                handle_collision(wall, pos, old_pos);
                collided = true;
            }
        }
    }

    if (new_sector && new_sector != old_sector)
    {
        for (u32 i = 0; i < new_sector->wall_count; i++)
        {
            const wall_t *wall = &new_sector->walls[i];
            if (!wall->is_solid) continue;
            if (line_segment_intersect(old_pos, *pos, wall->pos_start, wall->pos_end))
            {
                handle_collision(wall, pos, old_pos);
                collided = true;
            }
        }
    }

    return collided;
}

level_t level_load(const level_data_t* data)
{
    level_t level = {
        .name = data->name,
        .path = data->path,
        .sectors = NULL,
        .sector_count = 0
    };

    for (i32 s = 0; s < data->sector_count; s++)
    {
        const level_sector_data_t* sector_data = &data->sectors[s];

        sector_t new_sector = {
            .id = sector_data->id,
            .light_intensity = sector_data->light_intensity,
            .floor_height = sector_data->floor_height,
            .ceil_height = sector_data->ceil_height,
            .wall_count = sector_data->quad_count,
            .walls = malloc(sizeof(wall_t) * sector_data->quad_count)
        };

        for (i32 i = 0; i < sector_data->quad_count; i++)
        {
            new_sector.walls[i] = wall_from_quad(&sector_data->quads[i], 
                                                  sector_data->floor_height, 
                                                  sector_data->ceil_height);
        }

        level.sectors = realloc(level.sectors, sizeof(sector_t) * (level.sector_count + 1));
        level.sectors[level.sector_count] = new_sector;
        level.sector_count++;
    }

    printf("Loaded level '%s': %d sectors\n", data->name, level.sector_count);
    return level;
}

#endif // LEVEL_RENDERING
#endif // LEVEL_H
