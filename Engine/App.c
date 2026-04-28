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

static u32 g_post_vao = 0;
static u32 g_post_vbo = 0;
static u32 g_post_program = 0;

static void post_init()
{
    static const char* vs =
        "#version 330 core\n"
        "layout(location=0) in vec2 aPos;\n"
        "out vec2 vUV;\n"
        "void main(){\n"
        "    vUV = aPos * 0.5 + 0.5;\n"
        "    gl_Position = vec4(aPos, 0.0, 1.0);\n"
        "}\n";
    static const char* fs =
        "#version 330 core\n"
        "in vec2 vUV;\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D u_screen;\n"
        "uniform float u_levels;\n"
        "void main(){\n"
        "    vec3 c = texture(u_screen, vUV).rgb;\n"
        "    c = round(c * (u_levels - 1.0)) / (u_levels - 1.0);\n"
        "    FragColor = vec4(c, 1.0);\n"
        "}\n";

    g_post_program = create_program(vs, fs);

    static const f32 quad[] = { -1,-1, 1,-1, 1,1, -1,-1, 1,1, -1,1 };
    glGenVertexArrays(1, &g_post_vao);
    glGenBuffers(1, &g_post_vbo);
    glBindVertexArray(g_post_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_post_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindVertexArray(0);
}

void post_blit(const i32 src_w, const i32 src_h, const i32 dst_w, const i32 dst_h)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, dst_w, dst_h);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glUseProgram(g_post_program);
    glUniform1i(glGetUniformLocation(g_post_program, "u_screen"), 0);
    glUniform1f(glGetUniformLocation(g_post_program, "u_levels"), PALETTE_LEVELS);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_fbo_color);

    glBindVertexArray(g_post_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
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
    post_init();

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
    if (g_post_program) glDeleteProgram(g_post_program);
    if (g_post_vao)     glDeleteVertexArrays(1, &g_post_vao);
    if (g_post_vbo)     glDeleteBuffers(1, &g_post_vbo);
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
