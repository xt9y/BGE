#ifndef BGE_RUNTIME_H
#define BGE_RUNTIME_H

#include "Engine.h"
#include "util/types.h"

typedef struct engine_runtime {
    level_data_t level;
    char loader_name[128];
    bool gl_started;

    bool h_pressed;
    bool tab_pressed;
    bool shoot_pressed;
    bool g_pressed;
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
