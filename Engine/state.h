#ifndef BGE_STATE_H
#define BGE_STATE_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "cam.h"
#include "editor.h"
#include "text.h"
#include "util/types.h"

typedef enum {
    STATE_PLAYING = 0,
    STATE_EDITOR,
    STATE_EXIT
} state_e;

typedef struct {
    u32 program;
    GLint u_model;
    GLint u_view;
    GLint u_proj;
    GLint u_tint;
    GLint u_uv_scale;
} data_t;

typedef struct {
    i32 w, h;
    i32 ww, wh;
    f32 scale;
} framebuffer_t;

/*
 * state_t intentionally stores references to subsystem storage owned by the
 * active engine_runtime.  Nothing in here is independently heap-owned.
 */
typedef struct {
    bool cursor_locked;
    GLFWwindow* win;
    state_e id;
    f32 dt;

    framebuffer_t* fb;
    data_t* data;
    camera_t* cam;
    editor_t* editor;
    texture_registry_t* text;

    vertex_t text_vertices[MAX_TEXT_VERTICES];
    u32 text_vertex_count;
    bool debug_visible;
} state_t;

extern state_t* g_state;
#define state (*g_state)

#endif
