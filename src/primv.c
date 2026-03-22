#include "primv.h"
#include "gfx.h"
#include "util/math.h"
#include <stdio.h>
#include <string.h>

static primitive_t* primitive_alloc_slot(primitive_registry_t* reg)
{
    if (reg->count >= MAX_PRIMITIVES) {
        printf("Primitive registry full!\n");
        return 0;
    }
    return &reg->primitives[reg->count++];
}

void primitive_registry_init(primitive_registry_t* reg)
{
    memset(reg, 0, sizeof(*reg));
    reg->count = 0;
}

void primitive_registry_cleanup(primitive_registry_t* reg)
{
    if (!reg) return;
    for (i32 i = 0; i < reg->count; i++)
        primitive_destroy(&reg->primitives[i]);
    memset(reg, 0, sizeof(*reg));
}

primitive_t* primitive_create(primitive_registry_t* reg, const vec3s pos, const vec2s scale, const texture_t tex)
{
    if (!reg) return 0;
    primitive_t* prim = primitive_alloc_slot(reg);
    if (!prim) return 0;
    prim->pos = pos;
    prim->tex = tex;

    const f32 vertices[] = {
         // positions     // colors               // texture coords
         0.5f * scale.x,  0.5f * scale.y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,  // top right
         0.5f * scale.x, -0.5f * scale.y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,  // bottom right
        -0.5f * scale.x, -0.5f * scale.y, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,  // bottom left
        -0.5f * scale.x,  0.5f * scale.y, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,  // top left
    };

    const u32 indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    glGenVertexArrays(1, &prim->vao);
    glGenBuffers(1, &prim->vbo);
    glGenBuffers(1, &prim->ebo);

    glBindVertexArray(prim->vao);

    glBindBuffer(GL_ARRAY_BUFFER, prim->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, prim->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    // Texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    return prim;
}

void primitive_draw(primitive_t* prim, const u32 program)
{
    if (!prim) return;

    f32 model[16];
    mat4_translate(model, prim->pos.x, prim->pos.y, prim->pos.z);
    glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, model);

    texture_bind(&prim->tex, 0);
    texture_bind(&prim->tex, 1);

    glBindVertexArray(prim->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void primitive_destroy(primitive_t* prim)
{
    if (!prim) return;
    glDeleteVertexArrays(1, &prim->vao);
    glDeleteBuffers(1, &prim->vbo);
    glDeleteBuffers(1, &prim->ebo);
    prim->vao = prim->vbo = prim->ebo = 0;
}

void primitive_draw_all(primitive_registry_t* reg, const u32 program)
{
    if (!reg) return;
    for (i32 i = 0; i < reg->count; i++) primitive_draw(&reg->primitives[i], program);
}

void primitive_init(primitive_registry_t* reg, const texture_registry_t* reg_text)
{
    primitive_registry_init(reg);
    reg->primitives[0] = *primitive_create(reg, (vec3s){0.0f, 0.0f, 0.0f}, (vec2s){1.0f, 1.0f}, reg_text->textures[0]);
    reg->primitives[1] = *primitive_create(reg, (vec3s){0.0f, 1.0f, 0.0f}, (vec2s){4.0f, 1.0f}, reg_text->textures[1]);
}
