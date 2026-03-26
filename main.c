#include "Engine/App.h"
#include "Engine/util.h"

void RUN()
{
    GL_START();

    {   // Camera
        int fbw = WIDTH, fbh = HEIGHT;
        glfwGetFramebufferSize(state.win, &fbw, &fbh);
        state.cam->pos = (vec3s){3.0f, 2.0f, 6.0f};
        state.cam->front = (vec3s){0.0f, 0.0f, -1.0f};
        state.cam->up = (vec3s){0.0f, 1.0f, 0.0f};
        state.cam->yaw = -120.0f;
        state.cam->pitch = -20.0f;
        state.cam->lastX = (f32)fbw * 0.5f;
        state.cam->lastY = (f32)fbh * 0.5f;
        state.cam->firstMouse = true;
        update_camera_vectors(state.cam);
    }

    {   // Textures
        texture_registry_init(state.text);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/ground.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/stone.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/awesomeface.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create_solid(255, 0, 0);
        state.text->textures[state.text->count++] = *texture_create_solid(0, 255, 0);
        state.text->textures[state.text->count++] = *texture_create_solid(0, 0, 255);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/font.png", TEX_FILTER_NEAREST, TEX_WRAP_CLAMP_TO_EDGE);
        text_init(&state.text->textures[state.text->count - 1]);
    }

    {   // Primitives
        primitive_registry_init(state.prim);
#define T(_idx, _reg_text) (((_idx) >= 0 && (_idx) < (_reg_text)->count) ? &(_reg_text)->textures[(_idx)] : NULL)
        state.prim->primitives[state.prim->count++] = *primitive_create_quad((vec3s){2.0f, 2.0f, 0.0f},   (vec3s){0.0f, 0.0f, 0.0f},    (vec2s){4.0f, 4.0f},      T(9, state.text));
        state.prim->primitives[state.prim->count++] = *primitive_create_quad((vec3s){2.0f, 0.0f, 2.0f},   (vec3s){90.0f, 0.0f, 0.0f},   (vec2s){4.0f, 4.0f},      T(0, state.text));
        state.prim->primitives[state.prim->count++] = *primitive_create_quad((vec3s){0.0f, 2.0f, 2.0f},   (vec3s){0.0f, 90.0f, 0.0f},   (vec2s){4.0f, 4.0f},      T(1, state.text));
        state.prim->primitives[state.prim->count++] = *primitive_create_quad((vec3s){0.0f, 0.0f, 0.0f},   (vec3s){0.0f, 0.0f, 0.0f},    (vec2s){100.0f, 0.03f},   T(3, state.text));
        state.prim->primitives[state.prim->count++] = *primitive_create_quad((vec3s){0.0f, 0.0f, 0.0f},   (vec3s){90.0f, 90.0f, 90.0f}, (vec2s){0.03f, 100.0f},   T(4, state.text));
        state.prim->primitives[state.prim->count++] = *primitive_create_quad((vec3s){0.0f, 0.0f, 0.0f},   (vec3s){0.0f, 90.0f, 0.0f},   (vec2s){100.0f, 0.03f},   T(5, state.text));
        state.prim->primitives[state.prim->count++] = *primitive_create_quad((vec3s){2.0f, 2.0f, 2.0f},   (vec3s){90.0f, 90.0f, 90.0f}, (vec2s){2.0f, 2.0f},      T(2, state.text));
#undef T
    }

    while (GL_FRAME())
    {
        state.prim->primitives[6].rot.x += 0.2f; state.prim->primitives[6].rot.z += 0.2f; state.prim->primitives[6].rot.y += 0.2f;
        // state.prim->primitives[0].rot.z += 0.2f;
        // state.prim->primitives[1].rot.z += 0.2f;
        // state.prim->primitives[2].rot.x += 0.2f;
    }

    text_shutdown();
    GL_END();
}

void RENDER()
{
    int fbw = 0, fbh = 0;
    glfwGetFramebufferSize(state.win, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(state.data->program);

    f32 model[16], view[16], proj[16];
    mat4_identity(model);
    mat4_lookat(view, state.cam->pos, vec3_add(state.cam->pos, state.cam->front), state.cam->up);
    mat4_perspective(proj, DEG2RAD(45.0f), (f32)fbw / (f32)fbh, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "projection"), 1, GL_FALSE, proj);

    primitive_draw_all(state.prim, state.data->program);

    VK_BEGINTEXT;
    VK_DRAWTEXTF(10.0f, 10.0f, "FPS %.1f", GL_GETFPS());
    text_flush(fbw, fbh);
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
