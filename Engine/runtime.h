#ifndef BGE_RUNTIME_H
#define BGE_RUNTIME_H

#include "Engine.h"
#include "state.h"
#include "util/types.h"

typedef struct engine_runtime {
    /* Root-owned subsystem storage. */
    state_t state_storage;
    framebuffer_t framebuffer;
    data_t data;
    camera_t camera;
    editor_t editor;
    texture_registry_t textures;

    level_data_t level;
    char loader_name[128];
    bool platform_started;
    bool resources_started;
    bool editor_started;
    bool app_started;

    bool h_pressed;
    bool tab_pressed;
    bool e_pressed;
    bool n_pressed;
    bool x_pressed;
    bool r_pressed;
    bool v_pressed;
    bool enter_pressed;
    bool adj_pressed[3];
    f32 adj_timer[3];
} engine_runtime;

extern engine_runtime* bge_runtime;

void engine_input(void);
void engine_render(void);

#endif
