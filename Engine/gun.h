#ifndef GUN_H
#define GUN_H

#include "util/types.h"
#include "text.h"

#define MAX_WEAPONS 8
#define MAX_GUN_FRAMES 8

typedef struct {
    const char* tex_names[MAX_GUN_FRAMES];
    i32 tex_count;
    const char* flash_tex_name;
    vec2s gun_xy;
    f32 gun_size;
    f32 flash_size;
    vec2s flash_xy;
    f32 frame_duration;
} weapon_def_t;

typedef struct {
    weapon_def_t defs[MAX_WEAPONS];
    i32 count;
} gun_registry_t;

void gun_reg_init(gun_registry_t* reg);
void gun_init(void);
void gun_shutdown(void);
void gun_render(i32 rw, i32 rh);
void gun_shot(void);
i32 gun_get_current(void);
void gun_select(i32 idx);
void gun_next(void);
void gun_prev(void);

#endif
