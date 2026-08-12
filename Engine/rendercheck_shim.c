#include "App.h"
#include "rendercheck.h"

#define GL_FRAME bge_gl_frame_original
#define GL_END bge_gl_end_original
#include "App.c"
#undef GL_END
#undef GL_FRAME

int GL_FRAME(void)
{
    bge_rendercheck_gpu_begin();
    const int keep_running = bge_gl_frame_original();
    bge_rendercheck_gpu_end();
    return keep_running;
}

void GL_END(void)
{
    bge_rendercheck_gpu_shutdown();
    bge_gl_end_original();
}
