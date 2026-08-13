#include "platform.h"

#include "gfx.h"
#include "imgui_c.h"

#include <glad/glad.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__has_include)
#  if __has_include(<rendercheck/capture.h>) && __has_include(<rendercheck/metrics.h>)
#    define RENDERCHECK_AVAILABLE 1
#    include <rendercheck/capture.h>
#    include <rendercheck/metrics.h>
#  endif
#endif
#ifndef RENDERCHECK_AVAILABLE
#  define RENDERCHECK_AVAILABLE 0
#endif

state_t* g_state = NULL;

static bool g_glfw_initialized = false;
static bool g_imgui_initialized = false;
static f32 g_last_time = 0.0f;
static bool g_first_frame = true;
static double g_fps_accum = 0.0;
static u32 g_fps_frames = 0;
static double g_fps_value = 0.0;

static u32 g_fbo = 0;
static u32 g_fbo_color = 0;
static u32 g_fbo_depth_stencil = 0;
static i32 g_fbo_w = 0;
static i32 g_fbo_h = 0;

static u32 g_post_vao = 0;
static u32 g_post_vbo = 0;
static u32 g_post_program = 0;
static GLint g_post_u_screen = -1;
static GLint g_post_u_levels = -1;

static int rendercheck_enabled(void)
{
#if RENDERCHECK_AVAILABLE
    return getenv("RENDERCHECK") != NULL;
#else
    return 0;
#endif
}

#if RENDERCHECK_AVAILABLE
static GLuint g_rendercheck_gpu_query = 0;
static bool g_rendercheck_gpu_query_active = false;

static void rendercheck_gpu_begin(void)
{
    if (!rendercheck_enabled()) return;
    if (!g_rendercheck_gpu_query) glGenQueries(1, &g_rendercheck_gpu_query);
    if (!g_rendercheck_gpu_query) return;
    glBeginQuery(GL_TIME_ELAPSED, g_rendercheck_gpu_query);
    g_rendercheck_gpu_query_active = true;
}

static void rendercheck_gpu_end(void)
{
    if (!g_rendercheck_gpu_query_active) return;
    glEndQuery(GL_TIME_ELAPSED);
    g_rendercheck_gpu_query_active = false;

    GLuint64 elapsed_ns = 0;
    glGetQueryObjectui64v(g_rendercheck_gpu_query, GL_QUERY_RESULT, &elapsed_ns);
    if (rendercheck_gpu_ms((double)elapsed_ns / 1000000.0) < 0)
        fprintf(stderr, "RendererCheck: failed to write GPU metric\n");
}

static void rendercheck_capture_frame(GLFWwindow* window)
{
    if (!rendercheck_enabled() || !rendercheck_capture_requested()) return;

    int width = 0, height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    if (width <= 0 || height <= 0) return;

    const size_t row_bytes = (size_t)width * 3u;
    const size_t total_bytes = row_bytes * (size_t)height;
    unsigned char* pixels = (unsigned char*)malloc(total_bytes);
    if (!pixels) return;

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    for (int y = 0; y < height / 2; ++y) {
        unsigned char* top = pixels + (size_t)y * row_bytes;
        unsigned char* bottom = pixels + (size_t)(height - 1 - y) * row_bytes;
        for (size_t x = 0; x < row_bytes; ++x) {
            const unsigned char tmp = top[x];
            top[x] = bottom[x];
            bottom[x] = tmp;
        }
    }

    if (rendercheck_capture_rgb8(pixels, (uint32_t)width, (uint32_t)height, row_bytes) < 0)
        fprintf(stderr, "RendererCheck: failed to write frame capture\n");
    free(pixels);
}

static void rendercheck_gpu_shutdown(void)
{
    if (g_rendercheck_gpu_query) glDeleteQueries(1, &g_rendercheck_gpu_query);
    g_rendercheck_gpu_query = 0;
    g_rendercheck_gpu_query_active = false;
}
#else
static void rendercheck_gpu_begin(void) {}
static void rendercheck_gpu_end(void) {}
static void rendercheck_capture_frame(GLFWwindow* window) { (void)window; }
static void rendercheck_gpu_shutdown(void) {}
#endif

static void mouse_callback(GLFWwindow* window, const f64 xpos, const f64 ypos)
{
    (void)window;
    if (!g_state || !state.cursor_locked || !state.cam) return;
    camera_mouse_callback(state.cam, xpos, ypos);
}

static void framebuffer_size_callback(GLFWwindow* window, i32 width, i32 height)
{
    (void)window;
    if (!g_state || !state.fb) return;
    state.fb->w = width;
    state.fb->h = height;
}

static bool post_init(void)
{
    static const char* vs =
        "#version 330 core\n"
        "layout(location=0) in vec2 aPos;\n"
        "out vec2 vUV;\n"
        "void main(){ vUV=aPos*0.5+0.5; gl_Position=vec4(aPos,0.0,1.0); }\n";
    static const char* fs =
        "#version 330 core\n"
        "in vec2 vUV;\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D u_screen;\n"
        "uniform float u_levels;\n"
        "void main(){\n"
        "  vec3 c=texture(u_screen,vUV).rgb;\n"
        "  c=round(c*(u_levels-1.0))/(u_levels-1.0);\n"
        "  FragColor=vec4(c,1.0);\n"
        "}\n";

    g_post_program = create_program(vs, fs);
    if (!g_post_program) return false;

    g_post_u_screen = glGetUniformLocation(g_post_program, "u_screen");
    g_post_u_levels = glGetUniformLocation(g_post_program, "u_levels");
    if (g_post_u_screen < 0 || g_post_u_levels < 0) return false;

    static const f32 quad[] = { -1,-1, 1,-1, 1,1, -1,-1, 1,1, -1,1 };
    glGenVertexArrays(1, &g_post_vao);
    glGenBuffers(1, &g_post_vbo);
    if (!g_post_vao || !g_post_vbo) return false;

    glBindVertexArray(g_post_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_post_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glBindVertexArray(0);
    return true;
}

bool fbo_resize(const i32 w, const i32 h)
{
    if (w <= 0 || h <= 0) return false;
    if (g_fbo_w == w && g_fbo_h == h && g_fbo) return true;

    if (!g_fbo) glGenFramebuffers(1, &g_fbo);
    if (!g_fbo) return false;

    if (g_fbo_color) glDeleteTextures(1, &g_fbo_color);
    if (g_fbo_depth_stencil) glDeleteRenderbuffers(1, &g_fbo_depth_stencil);
    g_fbo_color = 0;
    g_fbo_depth_stencil = 0;

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
    const bool complete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (!complete) {
        fprintf(stderr, "BGE: framebuffer is incomplete\n");
        return false;
    }

    g_fbo_w = w;
    g_fbo_h = h;
    return true;
}

void post_blit(const i32 src_w, const i32 src_h, const i32 dst_w, const i32 dst_h)
{
    (void)src_w;
    (void)src_h;
    if (dst_w <= 0 || dst_h <= 0 || !g_post_program || !g_fbo_color) return;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, dst_w, dst_h);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glUseProgram(g_post_program);
    glUniform1i(g_post_u_screen, 0);
    glUniform1f(g_post_u_levels, PALETTE_LEVELS);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_fbo_color);
    glBindVertexArray(g_post_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}

u32 app_framebuffer(void)
{
    return g_fbo;
}

bool app_platform_init(void)
{
    if (!g_state || !state.fb || !state.data || !state.cam || !state.editor || !state.text)
        return false;

    if (!glfwInit()) {
        fprintf(stderr, "BGE: glfwInit failed\n");
        return false;
    }
    g_glfw_initialized = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    if (rendercheck_enabled()) glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    state.win = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
#ifndef __APPLE__
    if (!state.win) {
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
        state.win = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    }
#endif
    if (!state.win) {
        fprintf(stderr, "BGE: failed to create GLFW window\n");
        app_platform_shutdown();
        return false;
    }

    glfwMakeContextCurrent(state.win);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        fprintf(stderr, "BGE: failed to load OpenGL functions\n");
        app_platform_shutdown();
        return false;
    }

    if (!post_init()) {
        fprintf(stderr, "BGE: post-processing initialization failed\n");
        app_platform_shutdown();
        return false;
    }

    state.data->program = create_program(VS, FS);
    if (!state.data->program) {
        app_platform_shutdown();
        return false;
    }

    state.data->u_model = glGetUniformLocation(state.data->program, "model");
    state.data->u_view = glGetUniformLocation(state.data->program, "view");
    state.data->u_proj = glGetUniformLocation(state.data->program, "projection");
    state.data->u_tint = glGetUniformLocation(state.data->program, "u_tint");
    state.data->u_uv_scale = glGetUniformLocation(state.data->program, "u_uv_scale");
    const GLint u_texture = glGetUniformLocation(state.data->program, "texture1");
    if (state.data->u_model < 0 || state.data->u_view < 0 || state.data->u_proj < 0 ||
        state.data->u_tint < 0 || state.data->u_uv_scale < 0 || u_texture < 0) {
        fprintf(stderr, "BGE: required world shader uniform is missing\n");
        app_platform_shutdown();
        return false;
    }

    glUseProgram(state.data->program);
    glUniform1i(u_texture, 0);
    glUniform3f(state.data->u_tint, 1.0f, 1.0f, 1.0f);
    glUniform2f(state.data->u_uv_scale, 1.0f, 1.0f);

    glfwSwapInterval(0);
    glfwSetCursorPosCallback(state.win, mouse_callback);
    glfwSetFramebufferSizeCallback(state.win, framebuffer_size_callback);
    glfwSetInputMode(state.win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    f32 y_scale = 1.0f;
    glfwGetWindowContentScale(state.win, &state.fb->scale, &y_scale);
    glfwGetWindowSize(state.win, &state.fb->ww, &state.fb->wh);
    glfwGetFramebufferSize(state.win, &state.fb->w, &state.fb->h);

    imgui_init(state.win);
    g_imgui_initialized = true;
    imgui_set_mouse_enabled(false);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);

    state.id = STATE_PLAYING;
    state.cursor_locked = true;
    g_last_time = (f32)glfwGetTime();
    g_first_frame = true;
    g_fps_accum = 0.0;
    g_fps_frames = 0;
    g_fps_value = 0.0;
    return true;
}

bool app_frame_begin(void)
{
    if (!g_state || !state.win || state.id == STATE_EXIT || glfwWindowShouldClose(state.win))
        return false;

    rendercheck_gpu_begin();

    const f32 now = (f32)glfwGetTime();
    f32 dt = g_first_frame ? 0.0f : now - g_last_time;
    g_first_frame = false;
    g_last_time = now;
    if (dt < 0.0f) dt = 0.0f;
    if (dt > 0.1f) dt = 0.1f;
    state.dt = dt;

    glfwPollEvents();
    return true;
}

void app_frame_end(void)
{
    if (!g_state || !state.win) return;
    rendercheck_capture_frame(state.win);
    rendercheck_gpu_end();
    glfwSwapBuffers(state.win);
    if (rendercheck_enabled()) state.id = STATE_EXIT;
}

void app_platform_shutdown(void)
{
    if (!g_state) return;

    if (state.win) {
        if (g_fbo_color) glDeleteTextures(1, &g_fbo_color);
        if (g_fbo_depth_stencil) glDeleteRenderbuffers(1, &g_fbo_depth_stencil);
        if (g_fbo) glDeleteFramebuffers(1, &g_fbo);
        if (g_post_program) glDeleteProgram(g_post_program);
        if (g_post_vao) glDeleteVertexArrays(1, &g_post_vao);
        if (g_post_vbo) glDeleteBuffers(1, &g_post_vbo);
        if (state.data && state.data->program) glDeleteProgram(state.data->program);
        rendercheck_gpu_shutdown();

        if (g_imgui_initialized) imgui_shutdown();
        g_imgui_initialized = false;

        glfwDestroyWindow(state.win);
        state.win = NULL;
    }

    if (g_glfw_initialized) glfwTerminate();
    g_glfw_initialized = false;

    g_fbo = 0;
    g_fbo_color = 0;
    g_fbo_depth_stencil = 0;
    g_fbo_w = 0;
    g_fbo_h = 0;
    g_post_program = 0;
    g_post_vao = 0;
    g_post_vbo = 0;
    g_post_u_screen = -1;
    g_post_u_levels = -1;
    g_last_time = 0.0f;
    g_first_frame = true;
    g_fps_accum = 0.0;
    g_fps_frames = 0;
    g_fps_value = 0.0;

    if (state.data) *state.data = (data_t){0};
}

double app_get_fps(void)
{
    if (!g_state) return 0.0;
    g_fps_accum += (double)state.dt;
    g_fps_frames += 1;
    if (g_fps_accum >= 0.5) {
        g_fps_value = (double)g_fps_frames / g_fps_accum;
        g_fps_accum = 0.0;
        g_fps_frames = 0;
    }
    return g_fps_value;
}
