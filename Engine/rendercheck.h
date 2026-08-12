#pragma once

#include <stdio.h>
#include <stdlib.h>

static int rendercheck_enabled(void)
{
    return getenv("RENDERCHECK") != NULL;
}

static void rendercheck_window_hint(void)
{
    if (rendercheck_enabled()) glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
}

static int rendercheck_capture_frame(GLFWwindow* window)
{
    const char* path = getenv("RENDERCHECK_CAPTURE_PATH");
    if (!path || !path[0]) return 0;

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    if (width <= 0 || height <= 0) return -1;

    const size_t row_bytes = (size_t)width * 3u;
    const size_t image_bytes = row_bytes * (size_t)height;
    unsigned char* pixels = (unsigned char*)malloc(image_bytes);
    if (!pixels) return -1;

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadBuffer(GL_BACK);
    glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    FILE* out = fopen(path, "wb");
    if (!out) {
        free(pixels);
        return -1;
    }

    if (fprintf(out, "P6\n%d %d\n255\n", width, height) < 0) {
        fclose(out);
        free(pixels);
        return -1;
    }

    for (int y = height - 1; y >= 0; --y) {
        if (fwrite(pixels + (size_t)y * row_bytes, 1, row_bytes, out) != row_bytes) {
            fclose(out);
            free(pixels);
            return -1;
        }
    }

    const int close_result = fclose(out);
    free(pixels);
    return close_result == 0 ? 0 : -1;
}

static void rendercheck_after_render(GLFWwindow* window)
{
    if (rendercheck_enabled() && rendercheck_capture_frame(window) != 0)
        fprintf(stderr, "RendererCheck: failed to write frame capture\n");
}

static GLuint g_rendercheck_gpu_query = 0;
static int g_rendercheck_gpu_query_active = 0;

static void rendercheck_gpu_begin(void)
{
    if (!rendercheck_enabled()) return;
    if (!g_rendercheck_gpu_query) glGenQueries(1, &g_rendercheck_gpu_query);
    if (!g_rendercheck_gpu_query) return;
    glBeginQuery(GL_TIME_ELAPSED, g_rendercheck_gpu_query);
    g_rendercheck_gpu_query_active = 1;
}

static void rendercheck_gpu_end(void)
{
    if (!g_rendercheck_gpu_query_active) return;

    glEndQuery(GL_TIME_ELAPSED);
    g_rendercheck_gpu_query_active = 0;

    GLuint64 elapsed_ns = 0;
    glGetQueryObjectui64v(g_rendercheck_gpu_query, GL_QUERY_RESULT, &elapsed_ns);

    const char* path = getenv("RENDERCHECK_METRICS_PATH");
    if (!path || !path[0]) return;

    FILE* out = fopen(path, "a");
    if (!out) {
        fprintf(stderr, "RendererCheck: failed to open metrics file\n");
        return;
    }

    fprintf(out, "gpu_ms=%.9f\n", (double)elapsed_ns / 1000000.0);
    fclose(out);
}

static void rendercheck_gpu_shutdown(void)
{
    if (g_rendercheck_gpu_query) {
        glDeleteQueries(1, &g_rendercheck_gpu_query);
        g_rendercheck_gpu_query = 0;
    }
    g_rendercheck_gpu_query_active = 0;
}
