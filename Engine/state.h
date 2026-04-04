#ifndef STATE_H
#define STATE_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "cam.h"
#include "text.h"
#include "util/types.h"

#include "editor.h"
#include "level.h"

typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_EDITOR,
    STATE_EXIT
} state_e;

typedef struct {
    u32 vao, vbo, ebo, program;
} data_t;

typedef struct {
    i32 w, h;
} framebuffer_t;

typedef struct {
    bool cursor_locked;
    GLFWwindow* win;
    state_e id;
    const char* toString[4];
    f32 dt;
    framebuffer_t* fb;
    data_t* data;
    camera_t* cam;
    
    editor_t* editor;

    texture_registry_t* text;

    vertex_t text_vertices[MAX_TEXT_VERTICES];
    u32 text_vertex_count;

    int level_id;
    int level_count;
    level_data_t levels[MAX_LEVELS];

    vertex_t wall_vertices[MAX_WALL_VERTICES];
    uint32_t wall_vertex_count;
} state_t;

extern state_t state;

#endif
