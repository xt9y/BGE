#include "prim.h"
#include "gfx.h"
#include "util/math.h"
#include <stdio.h>
#include <string.h>

#define FALLBACK_TEXTURE (*texture_get_fallback())

static primitive_registry_t g_registry;

static primitive_t* primitive_alloc_slot(void)
{
    if (g_registry.count >= MAX_PRIMITIVES) {
        printf("Primitive registry full!\n");
        return 0;
    }
    return &g_registry.primitives[g_registry.count++];
}

void primitive_registry_init(primitive_registry_t* reg)
{
    memset(reg, 0, sizeof(*reg));
    reg->count = 0;

    memset(&g_registry, 0, sizeof(g_registry));
    g_registry.count = 0;
}

void primitive_registry_cleanup(primitive_registry_t* reg)
{
    if (!reg) return;
    for (i32 i = 0; i < reg->count; i++) primitive_destroy(&reg->primitives[i]);
    memset(reg, 0, sizeof(*reg));
    memset(&g_registry, 0, sizeof(g_registry));
}

primitive_t* primitive_create_quad(const vec3s pos, const vec3s rot, const vec2s size, const texture_t* tex)
{
    primitive_t* prim = primitive_alloc_slot();
    if (!prim) return 0;
    prim->rot = rot;
    prim->pos = pos;
    prim->tex = tex ? *tex : FALLBACK_TEXTURE;

    const f32 half_x = 0.5f * size.x;
    const f32 half_y = 0.5f * size.y;
    const f32 u_repeat = 6.0f, v_repeat = u_repeat;
    const f32 vertices[] = {
         // positions             // colors           // texture coords
         half_x,  half_y, 0.0f,   1.0f, 1.0f, 1.0f,   u_repeat, v_repeat,  // top right
         half_x, -half_y, 0.0f,   1.0f, 1.0f, 1.0f,   u_repeat, 0.0f,      // bottom right
        -half_x, -half_y, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f,     0.0f,      // bottom left
        -half_x,  half_y, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f,     v_repeat,  // top left
    };

    const u32 indices[] = { 0, 1, 3, 1, 2, 3 };

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

    f32 rot_x[16], rot_y[16], rot_z[16], rot_yx[16], rot_xyz[16], model[16];

    mat4_rotate_x(rot_x, DEG2RAD(prim->rot.x));
    mat4_rotate_y(rot_y, DEG2RAD(prim->rot.y));
    mat4_rotate_z(rot_z, DEG2RAD(prim->rot.z));
    mat4_multiply(rot_yx, rot_y, rot_x);
    mat4_multiply(rot_xyz, rot_z, rot_yx);

    memcpy(model, rot_xyz, sizeof(model));
    model[12] = prim->pos.x;
    model[13] = prim->pos.y;
    model[14] = prim->pos.z;

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

