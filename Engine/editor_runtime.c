#include "App.h"
#include "editor.h"
#include "imgui_c.h"
#include "state.h"

#include <glad/glad.h>
#include <string.h>

static bool bge_editor_button(const char* label)
{
    if (label && strcmp(label, "Create New Level") == 0) return false;
    return imgui_button(label);
}

static bool bge_editor_combo(const char* label, i32* current_idx,
    const char* const* items, i32 items_count)
{
    if (label && strcmp(label, "Level") == 0) return false;
    return imgui_combo(label, current_idx, items, items_count);
}

void bge_legacy_editor_save(level_data_t* level);

#define imgui_button bge_editor_button
#define imgui_combo bge_editor_combo
#define editor_add_level bge_legacy_editor_add_level
#define editor_switch_level bge_legacy_editor_switch_level
#define editor_save bge_legacy_editor_save
#include "editor.c"
#undef editor_save
#undef editor_switch_level
#undef editor_add_level
#undef imgui_combo
#undef imgui_button

void editor_init(void)
{
    if (!state.editor) return;
    level_data_t* level = state.editor->level;
    memset(state.editor, 0, sizeof(*state.editor));
    state.editor->level = level;
    if (state.cam) state.editor->template_quad = get_default_quad(state.cam);
    state.editor->template_mods = EDITOR_MOD_NONE;
}

void editor_shutdown(void)
{
    if (g_editor_ebo) glDeleteBuffers(1, &g_editor_ebo);
    if (g_editor_vbo) glDeleteBuffers(1, &g_editor_vbo);
    if (g_editor_vao) glDeleteVertexArrays(1, &g_editor_vao);
    g_editor_ebo = 0;
    g_editor_vbo = 0;
    g_editor_vao = 0;
    g_editor_vao_initialized = false;

    if (state.editor) {
        state.editor->selected_quad = NULL;
        state.editor->selected_sector = NULL;
        state.editor->portal_link_source = NULL;
        state.editor->ui_wall_window_count = 0;
    }
}
