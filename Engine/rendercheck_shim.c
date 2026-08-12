#include "App.h"
#include "rendercheck.h"

#define GL_FRAME rendercheck_gl_frame_original
#define GL_END rendercheck_gl_end_original
#include "App.c"
#undef GL_END
#undef GL_FRAME

int GL_FRAME(void)
{
    rendercheck_gpu_begin();
    const int keep_running = rendercheck_gl_frame_original();
    rendercheck_gpu_end();
    return keep_running;
}

void GL_END(void)
{
    rendercheck_gpu_shutdown();
    rendercheck_gl_end_original();
}
