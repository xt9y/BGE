#include "App.h"
#include "util.h"

#include "cam.h"
#include "gfx.h"
#include "prim.h"
#include "state.h"
#include "text.h"
#include "util/math.h"

#include <stdio.h>
#include <stdlib.h>

static f32 g_last_time = 0.0f;

state_t state;

static void mouse_callback(GLFWwindow* w, const f64 xpos, const f64 ypos)
{
    (void)w; if (!state.cursor_locked) return;
    camera_mouse_callback(state.cam, xpos, ypos);
}

static void framebuffer_size_callback(GLFWwindow* w, const int width, const int height)
{
    (void)w; if (width <= 0 || height <= 0) return;
    glViewport(0, 0, width, height);
}

void GL_START(void)
{
    state = (state_t){0};
    state.cursor_locked = true;
    
    ASSERT(glfwInit());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);

    state.win = glfwCreateWindow(WIDTH, HEIGHT, TITLE, 0, 0);
    ASSERT(state.win);

    glfwMakeContextCurrent(state.win);
    glfwSetFramebufferSizeCallback(state.win, framebuffer_size_callback);
    glfwSetCursorPosCallback(state.win, mouse_callback);
    glfwSetInputMode(state.win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    int fbw = 0, fbh = 0;
    glfwGetFramebufferSize(state.win, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);

    state.data = malloc(sizeof(*state.data));
    state.cam  = malloc(sizeof(*state.cam));  memset(state.cam,  0, sizeof(*state.cam));
    state.text = malloc(sizeof(*state.text)); memset(state.text, 0, sizeof(*state.text));
    state.prim = malloc(sizeof(*state.prim)); memset(state.prim, 0, sizeof(*state.prim));
    ASSERT(state.data && state.cam && state.text && state.prim);

    state.data->program = create_program(VS, FS);

    glUseProgram(state.data->program);
    glUniform1i(glGetUniformLocation(state.data->program, "texture1"), 0);
    glUniform1i(glGetUniformLocation(state.data->program, "texture2"), 1);

    glEnable(GL_DEPTH_TEST);
    state.id = STATE_PLAYING;
    g_last_time = (f32)glfwGetTime();
}

int GL_FRAME(void)
{
    const f32 now = (f32)glfwGetTime();
    state.dt = now - g_last_time;
    g_last_time = now;

    INPUT();
    RENDER();

    glfwSwapBuffers(state.win);
    glfwPollEvents();

    return !glfwWindowShouldClose(state.win) && state.id != STATE_EXIT;
}

void GL_END(void)
{
    if (state.text) texture_registry_cleanup(state.text);
    glfwTerminate();
    if (state.prim) primitive_registry_cleanup(state.prim);
    if (state.data) glDeleteProgram(state.data->program);
    if (state.text) free(state.text);
    if (state.prim) free(state.prim);
    if (state.cam) free(state.cam);
    if (state.data) free(state.data);
    state.text = 0;
    state.prim = 0;
    state.cam = 0;
    state.data = 0;
}
