#ifndef PRIMV_H
#define PRIMV_H

#include "text.h"
#include "util/types.h"

#define MAX_PRIMITIVES 128

typedef struct {
    u32 vao, vbo, ebo;
    vec3s pos;
    texture_t tex;
} primitive_t;

typedef struct {
    primitive_t primitives[MAX_PRIMITIVES];
    i32 count;
} primitive_registry_t;

// void primitive_registry_init(primitive_registry_t* reg);
void primitive_registry_cleanup(primitive_registry_t* reg);
// primitive_t* primitive_create(primitive_registry_t* reg, vec3s pos, vec2s scale, texture_t tex);
// void primitive_draw(primitive_t* prim, u32 program);
void primitive_draw_all(primitive_registry_t* reg, u32 program);
void primitive_destroy(primitive_t* prim);

void primitive_init(primitive_registry_t* reg, const texture_registry_t* reg_text);

#endif
