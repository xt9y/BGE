#include "Engine/App.h"
#include "Engine/util.h"

void RUN()
{
    GL_START();

    {   // Camera
        state.cam->pos = (vec3s){3.0f, 2.0f, 6.0f};
        state.cam->front = (vec3s){0.0f, 0.0f, -1.0f};
        state.cam->up = (vec3s){0.0f, 1.0f, 0.0f};
        state.cam->yaw = -120.0f;
        state.cam->pitch = -20.0f;
        state.cam->lastX = WIDTH * 0.5f;
        state.cam->lastY = HEIGHT * 0.5f;
        state.cam->firstMouse = true;
        update_camera_vectors(state.cam);
    }

    {   // Textures
        texture_registry_init(state.text);
        state.text->textures[0] = *texture_create("Engine/res/ground.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[1] = *texture_create("Engine/res/stone.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[2] = *texture_create("Engine/res/awesomeface.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[7] = *texture_create_solid(255, 0, 0);
        state.text->textures[8] = *texture_create_solid(0, 255, 0);
        state.text->textures[9] = *texture_create_solid(0, 0, 255);
    }

    {   // Primitives
        primitive_registry_init(state.prim);
#define T(_idx, _reg_text) (((_idx) >= 0 && (_idx) < MAX_TEXTURES) ? &(_reg_text)->textures[(_idx)] : NULL)
        primitive_create_quad(state.prim,  (vec3s){2.0f, 2.0f, 0.0f},   (vec3s){0.0f, 0.0f, 0.0f},    (vec2s){4.0f, 4.0f},      T(-1, state.text));
        primitive_create_quad(state.prim,  (vec3s){2.0f, 0.0f, 2.0f},   (vec3s){90.0f, 0.0f, 0.0f},   (vec2s){4.0f, 4.0f},      T(0, state.text));
        primitive_create_quad(state.prim,  (vec3s){0.0f, 2.0f, 2.0f},   (vec3s){0.0f, 90.0f, 0.0f},   (vec2s){4.0f, 4.0f},      T(1, state.text));
        primitive_create_quad(state.prim,  (vec3s){0.0f, 0.0f, 0.0f},   (vec3s){0.0f, 0.0f, 0.0f},    (vec2s){100.0f, 0.03f},   T(7, state.text));
        primitive_create_quad(state.prim,  (vec3s){0.0f, 0.0f, 0.0f},   (vec3s){90.0f, 90.0f, 90.0f}, (vec2s){0.03f, 100.0f},   T(8, state.text));
        primitive_create_quad(state.prim,  (vec3s){0.0f, 0.0f, 0.0f},   (vec3s){0.0f, 90.0f, 0.0f},   (vec2s){100.0f, 0.03f},   T(9, state.text));
#undef T
    }

    while (GL_FRAME())
    {

    }

    GL_END();
}

void RENDER()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(state.data->program);

    f32 model[16], view[16], proj[16];
    mat4_identity(model);
    mat4_lookat(view, state.cam->pos, vec3_add(state.cam->pos, state.cam->front), state.cam->up);
    mat4_perspective(proj, DEG2RAD(45.0f), (f32)WIDTH / (f32)HEIGHT, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "projection"), 1, GL_FALSE, proj);

    primitive_draw_all(state.prim, state.data->program);
}

void INPUT()
{
    if (glfwGetKey(state.win, GLFW_KEY_ESCAPE) == GLFW_PRESS) state.id = STATE_EXIT;

    const bool shift_active = glfwGetKey(state.win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    if (shift_active && state.cursor_locked) {
        glfwSetInputMode(state.win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        state.cursor_locked = false;
    } else if (!shift_active && !state.cursor_locked) {
        glfwSetInputMode(state.win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        state.cursor_locked = true;
        state.cam->firstMouse = true;
    }

    if (state.cursor_locked)
    {
        if (!state.cam || !state.win) return;
        const f32 speed = 2.5f * state.dt;
        const vec3s right = vec3_normalize(vec3_cross(state.cam->front, state.cam->up));

        if (glfwGetKey(state.win, GLFW_KEY_W) == GLFW_PRESS) state.cam->pos = vec3_add(state.cam->pos, vec3_scale(state.cam->front, speed));
        if (glfwGetKey(state.win, GLFW_KEY_S) == GLFW_PRESS) state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(state.cam->front, speed));
        if (glfwGetKey(state.win, GLFW_KEY_A) == GLFW_PRESS) state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(right, speed));
        if (glfwGetKey(state.win, GLFW_KEY_D) == GLFW_PRESS) state.cam->pos = vec3_add(state.cam->pos, vec3_scale(right, speed));
    }
}

ENGINE_ENTRY_POINT
