#include "app_hooks.h"
#include "level.h"
#include "render.h"
#include "state.h"
#include "text.h"

#include <stdio.h>

bool engine_resources_init(void)
{
    if (!g_state || !state.text) return false;

    if (!texture_registry_init(state.text)) {
        fprintf(stderr, "BGE: texture registry initialization failed\n");
        return false;
    }

    if (!bge_app_register_resources()) {
        fprintf(stderr, "BGE: application resource registration failed\n");
        texture_registry_cleanup(state.text);
        return false;
    }

    if (!text_init()) {
        fprintf(stderr, "BGE: text renderer initialization failed\n");
        texture_registry_cleanup(state.text);
        return false;
    }

    if (!render_init()) {
        fprintf(stderr, "BGE: world renderer initialization failed\n");
        text_shutdown();
        texture_registry_cleanup(state.text);
        return false;
    }

    return true;
}

void engine_resources_shutdown(void)
{
    if (!g_state) return;
    level_renderer_shutdown();
    render_shutdown();
    text_shutdown();
    if (state.text) texture_registry_cleanup(state.text);
}
