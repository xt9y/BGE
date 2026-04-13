#include "Engine/level.h"
#include "Engine/editor.h"
#include "Engine/App.h"

#include "Engine/res/level1.h"
#include "Engine/res/level2.h"
#include "Engine/res/level3.h"
#include "Engine/res/level4.h"

/*
 * TL:DR
 * > start with 'make'
 * > press b to switch between all levels
 * > press e to enter editor mode
 * > walk look around w a s d + mouse
 */

/* ok you see the way I make level loading and all of that using Engine/res/level(1/2/3/4).h,            
 * which are then exactly like this loaded into an levels array and then set as editor->level all          
 * inside main.c, my question is now how one would make an EXTERNAL EDITOR inside editor.c and             
 * editor.h, so i want it i change level inside the external EDITOR and then start the game and its        
 * updated the level, and the external level HAS to save the level in the current format that i save       
 * my levels in Engine/res/level1.h, 100% same style, no diffrences !!! if you have any questions          
 * about hotkeys or how the file should be structured again PLEASE ASK me and you have to KEEP the         
 * .h file strcuture of levels 100% the same dont change a thing about that, keep the level loading         
 * and level files 100% like they are right now     
 * */

void RUN()
{
    GL_START();

    {   // Camera
        state.fb->w = WIDTH, state.fb->h = HEIGHT;
        glfwGetFramebufferSize(state.win, &state.fb->w, &state.fb->h);
        state.cam->pos = (vec3s){9.8f, 6.4f, 13.6f};
        state.cam->front = (vec3s){0.0f, 0.0f, -1.0f};
        state.cam->up = (vec3s){0.0f, 1.0f, 0.0f};
        state.cam->yaw = 235.0f;
        state.cam->pitch = -16.0f;
        state.cam->lastX = (f32)state.fb->w * 0.5f;
        state.cam->lastY = (f32)state.fb->h * 0.5f;
        state.cursor_locked = false;
        update_camera_vectors(state.cam);
    }

    {   // Textures
        texture_registry_init(state.text);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/ground.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/stone.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create("Engine/res/awesomeface.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
        state.text->textures[state.text->count++] = *texture_create_solid(255, 255, 255);
        text_init(NULL);
    }

    {   // Levels 
        state.level_count = 0;
        state.levels[state.level_count++] = load_1();
        // state.levels[state.level_count++] = load_2();
        // state.levels[state.level_count++] = load_3();
        state.levels[state.level_count++] = load_4();    }

    {   // Editor
        state.editor->level = &state.levels[0];
    }

    while (GL_FRAME()) 
    {
        state.editor->level = &state.levels[state.level_id];
        
        if (state.id == STATE_EDITOR) 
        {
            editor_update();
        }
    }

    editor_save(state.editor->level);

#define END() do { GL_END(); } while (0)
    END();
}

void RENDER()
{
    state.fb->w = 0, state.fb->h = 0;
    glfwGetFramebufferSize(state.win, &state.fb->w, &state.fb->h);
    glViewport(0, 0, state.fb->w, state.fb->h);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(state.data->program);
    
    f32 model[16], view[16], proj[16];
    mat4_identity(model);
    mat4_lookat(view, state.cam->pos, vec3_add(state.cam->pos, state.cam->front), state.cam->up);
    mat4_perspective(proj, DEG2RAD(45.0f), (f32)state.fb->w / (f32)state.fb->h, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "model"), 1, GL_FALSE, model);
    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "projection"), 1, GL_FALSE, proj);

    text_begin();
    level_render(state.editor->level);
    if (state.id == STATE_EDITOR) {
        editor_render_look_at_info();
        editor_render();
        // Crosshair
        text_draw((vec2s){(f32)state.fb->w * 0.5f - 5.0f, (f32)state.fb->h * 0.5f - 10.0f}, "+");
    }

    text_draw((vec2s){10.0f, 10.0f}, ":;<=>? 0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ _ abcdefghijklmnopqrstuvwxyz. ");
    text_draw((vec2s){10.0f, 30.0f}, "FPS %.1f", GL_GETFPS());
    text_draw((vec2s){10.0f, 50.0f}, "POS: %.1f %.1f %.1f ; YAW %.1f ; PITCH %.1f", state.cam->pos.x, state.cam->pos.y, state.cam->pos.z, state.cam->yaw, state.cam->pitch);
    text_draw((vec2s){10.0f, 70.0f}, "CURRENT_LVL: %d ; MAX_LVLS: %d", state.level_id + 1, state.level_count);
    text_draw((vec2s){10.0f, 90.0f}, "STATE: %s", state.toString[state.id]);
    text_flush(state.fb->w, state.fb->h);
}

void ENGINE_INPUT()
{
    glfwSetInputMode(state.win, GLFW_CURSOR, state.cursor_locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    
    {   // Free mouse
        static bool tab_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_TAB) == GLFW_PRESS) {
            if (!tab_pressed) 
            {
                state.cursor_locked = !state.cursor_locked;
                if (state.cursor_locked) {
                    glfwSetCursorPos(state.win, state.fb->w * 0.5f, state.fb->h * 0.5f);
                    state.cam->firstMouse = true;
                }
                tab_pressed = true;
            }
        }
        else tab_pressed = false;
    }

    {   // Toggle editor
        static bool e_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_E) == GLFW_PRESS) {
            if (!e_pressed)
            {
                state.id = state.id == STATE_EDITOR ? STATE_PLAYING : STATE_EDITOR;
                e_pressed = true;
            }
        }
        else e_pressed = false;
    }

    {   // Next level
        static bool b_pressed = false;
        if (glfwGetKey(state.win, GLFW_KEY_B) == GLFW_PRESS)
        {
            if (!b_pressed)
            {
                editor_save(state.editor->level);
                state.level_id = (state.level_id + 1) % state.level_count;
                b_pressed = true;
            }
        }
        else b_pressed = false;
    }

    if (glfwGetKey(state.win, GLFW_KEY_ESCAPE) == GLFW_PRESS) state.id = STATE_EXIT;

    if (state.cursor_locked && state.id == STATE_PLAYING)
    {
        const f32 speed = 2.5f * state.dt;
        const vec3s right = vec3_normalize(vec3_cross(state.cam->front, state.cam->up));

        if (glfwGetKey(state.win, GLFW_KEY_W) == GLFW_PRESS) state.cam->pos = vec3_add(state.cam->pos, vec3_scale(state.cam->front, speed));
        if (glfwGetKey(state.win, GLFW_KEY_S) == GLFW_PRESS) state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(state.cam->front, speed));
        if (glfwGetKey(state.win, GLFW_KEY_A) == GLFW_PRESS) state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(right, speed));
        if (glfwGetKey(state.win, GLFW_KEY_D) == GLFW_PRESS) state.cam->pos = vec3_add(state.cam->pos, vec3_scale(right, speed));
    }

    if (state.cursor_locked && state.id == STATE_EDITOR)
    {
        const f32 speed = 4.5f * state.dt;
        const vec3s right = vec3_normalize(vec3_cross(state.cam->front, state.cam->up));

        if (glfwGetKey(state.win, GLFW_KEY_W) == GLFW_PRESS) state.cam->pos = vec3_add(state.cam->pos, vec3_scale(state.cam->front, speed));
        if (glfwGetKey(state.win, GLFW_KEY_S) == GLFW_PRESS) state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(state.cam->front, speed));
        if (glfwGetKey(state.win, GLFW_KEY_A) == GLFW_PRESS) state.cam->pos = vec3_sub(state.cam->pos, vec3_scale(right, speed));
        if (glfwGetKey(state.win, GLFW_KEY_D) == GLFW_PRESS) state.cam->pos = vec3_add(state.cam->pos, vec3_scale(right, speed));
    }
}

ENGINE_ENTRY_POINT
