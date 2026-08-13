#ifndef BGE_ENGINE_H
#define BGE_ENGINE_H

#include <stdbool.h>
#include "level.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Public BGE handle. The implementation owns all runtime subsystem storage and
 * may be initialized/destroyed repeatedly in the same process. One active BGE
 * window/context is supported at a time.
 */
typedef struct Engine {
    void* impl;
} Engine;

bool engine_init_impl(Engine* app, const char* level_path, const char* loader_expr, level_data_t level);
void engine_run(Engine* app);
void engine_destroy(Engine* app);

/* Stringizing preserves the generated loader name for level persistence. */
#define engine_init(app, level_path, loader_call) \
    engine_init_impl((app), (level_path), #loader_call, (loader_call))

#ifdef __cplusplus
}
#endif

#endif
