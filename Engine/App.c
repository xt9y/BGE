#include "App.h"
#include "util.h"

#include "cam.h"
#include "gfx.h"
#include "state.h"
#include "text.h"
#include "util.h"

#include <stdio.h>
#include <stdlib.h>

static f32    g_last_time  = 0.0f;
static double g_fps_accum  = 0.0;
static u32    g_fps_frames = 0;
static double g_fps_value  = 0.0;

u32 g_fbo               = 0;
static u32 g_fbo_color         = 0;
static u32 g_fbo_depth_stencil = 0;
static i32 g_fbo_w             = 0;
static i32 g_fbo_h             = 0;

state_t state;

void fbo_resize(const i32 w, const i32 h)
{
    if (g_fbo_w == w && g_fbo_h == h) return;
    g_fbo_w = w;
    g_fbo_h = h;

    if (!g_fbo) glGenFramebuffers(1, &g_fbo);

    if (g_fbo_color)         glDeleteTextures(1, &g_fbo_color);
    if (g_fbo_depth_stencil) glDeleteRenderbuffers(1, &g_fbo_depth_stencil);

    glGenTextures(1, &g_fbo_color);
    glBindTexture(GL_TEXTURE_2D, g_fbo_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glGenRenderbuffers(1, &g_fbo_depth_stencil);
    glBindRenderbuffer(GL_RENDERBUFFER, g_fbo_depth_stencil);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);

    glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_fbo_color, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, g_fbo_depth_stencil);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void mouse_callback(GLFWwindow* w, const f64 xpos, const f64 ypos)
{
    (void)w; if (!state.cursor_locked) return;
    camera_mouse_callback(state.cam, xpos, ypos);
}

void GL_START()
{
    state = (state_t){0};
    state.cursor_locked = true;

    ASSERT(glfwInit());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
#if defined(GLFW_PLATFORM_X11)
    glfwWindowHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif

    state.win = glfwCreateWindow(WIDTH, HEIGHT, TITLE, 0, 0);
    ASSERT(state.win);

    glfwMakeContextCurrent(state.win);

    ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

    glfwSwapInterval(0);
    glfwSetCursorPosCallback(state.win, mouse_callback);
    glfwSetInputMode(state.win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    state.fb        = malloc(sizeof(*state.fb));         memset(state.fb,       0, sizeof(*state.fb));
    state.data      = malloc(sizeof(*state.data));       memset(state.data,     0, sizeof(*state.data));
    state.cam       = malloc(sizeof(*state.cam));        memset(state.cam,      0, sizeof(*state.cam));
    state.text      = malloc(sizeof(*state.text));       memset(state.text,     0, sizeof(*state.text));
    state.editor    = malloc(sizeof(*state.editor));     memset(state.editor,   0, sizeof(*state.editor));
    ASSERT(state.fb && state.data && state.cam && state.text && state.editor);

    state.data->program = create_program(VS, FS);
    state.data->u_model = glGetUniformLocation(state.data->program, "model");
    state.data->u_view  = glGetUniformLocation(state.data->program, "view");
    state.data->u_proj  = glGetUniformLocation(state.data->program, "projection");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    state.id = STATE_PLAYING;
    g_last_time = (f32)glfwGetTime();
}

int GL_FRAME()
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

void GL_END()
{
    if (g_fbo_color)         glDeleteTextures(1, &g_fbo_color);
    if (g_fbo_depth_stencil) glDeleteRenderbuffers(1, &g_fbo_depth_stencil);
    if (g_fbo)               glDeleteFramebuffers(1, &g_fbo);

    if (state.text) texture_registry_cleanup(state.text);
    text_shutdown();
    glfwTerminate();
    if (state.data) glDeleteProgram(state.data->program);
    if (state.text) free(state.text);
    if (state.cam)  free(state.cam);
    if (state.data) free(state.data);
    if (state.fb)   free(state.fb);
    if (state.editor) free(state.editor);
    state.text   = 0;
    state.cam    = 0;
    state.data   = 0;
    state.fb     = 0;
}

double GL_GETFPS()
{
    g_fps_accum  += (double)state.dt;
    g_fps_frames += 1;
    if (g_fps_accum >= 0.5) {
        g_fps_value  = (double)g_fps_frames / g_fps_accum;
        g_fps_accum  = 0.0;
        g_fps_frames = 0;
    }
    return g_fps_value;
}
