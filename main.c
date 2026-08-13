#include "Engine/Engine.h"
#include "Engine/res/level1.h"

int main(void)
{
    Engine app = {0};
    if (!engine_init(&app, "Engine/res/level1.h", load_1())) return 1;
    engine_run(&app);
    engine_destroy(&app);
    return 0;
}
