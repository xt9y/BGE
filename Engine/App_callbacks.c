#include "runtime.h"

void level_guard_runtime_ids(level_data_t* level);

void INPUT(void)
{
    if (bge_runtime) level_guard_runtime_ids(&bge_runtime->level);
    engine_input();
}

void RENDER(void)
{
    if (bge_runtime) level_guard_runtime_ids(&bge_runtime->level);
    engine_render();
}
