#include "runtime.h"
#include "App.h"
#include "editor.h"
#include "imgui_c.h"
#include "render.h"
#include "state.h"
#include "text.h"

#include <glad/glad.h>

void engine_render(void)
{
    if (!bge_runtime || !state.win || !state.fb) return;

    i32 fbw = 0;
    i32 fbh = 0;
    glfwGetFramebufferSize(state.win, &fbw, &fbh);
    glfwGetWindowSize(state.win, &state.fb->ww, &state.fb->wh);
    if (fbw <= 0 || fbh <= 0) return;

    const f32 aspect = (f32)fbw / (f32)fbh;
    const i32 rw = RENDER_BASE_W;
    const i32 rh = (i32)((f32)RENDER_BASE_W / aspect);
    if (rh <= 0) return;

    render_main(rw, rh);
    post_blit(rw, rh, fbw, fbh);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glViewport(0, 0, fbw, fbh);
    glDisable(GL_DEPTH_TEST);

    imgui_newframe();

    text_begin();
    text_draw((vec2s){(f32)state.fb->ww * 0.5f - 5.0f,
                      (f32)state.fb->wh * 0.5f - 10.0f}, "+");
    text_flush(state.fb->ww, state.fb->wh);

    if (state.id == STATE_EDITOR) editor_ui();

    imgui_render();
    glEnable(GL_DEPTH_TEST);
}
