#ifndef BGE_RUNTIME_H
#define BGE_RUNTIME_H
#include "Engine.h"
typedef struct engine_runtime engine_runtime;
extern engine_runtime* bge_runtime;
void engine_input(void);
void engine_render(void);
#endif
