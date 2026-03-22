#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "state.h"
#include "gfx.h"
#include "text.h"
#include "util/math.h"
#include "cam.h"

#define WIDTH 1980
#define HEIGHT 1270

f32 g_lastTime;
state_t state;

// random callbacks
void mouse_callback(GLFWwindow* window, f64 xpos, f64 ypos) { camera_mouse_callback(xpos, ypos); }
void framebuffer_size_callback(GLFWwindow* w, i32 width, i32 height) { glViewport(0, 0, width, height); }

#define process_input() do { \
    if(glfwGetKey(state.win, GLFW_KEY_ESCAPE) == GLFW_PRESS) state.id=STATE_EXIT; \
    camera_update(); \
} while (0)

i32 init()
{
    ASSERT(glfwInit());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    state.win = glfwCreateWindow(WIDTH, HEIGHT, "opengl _f", 0, 0);
    ASSERT(state.win);

    glfwMakeContextCurrent(state.win);
    glfwSetFramebufferSizeCallback(state.win, framebuffer_size_callback);
    glfwSetCursorPosCallback(state.win, mouse_callback);
    glfwSetInputMode(state.win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    state.data = malloc(sizeof(*state.data));
    state.text = malloc(sizeof(*state.text)); memset(state.text, 0, sizeof(*state.text));
    state.prim = malloc(sizeof(*state.prim)); memset(state.prim, 0, sizeof(*state.prim));

    state.data->program = create_program(VS, FS);

    // Initialize Registry + Create textures
    texture_init(state.text);

    // Initialize Registry + Create primitives
    primitive_init(state.prim, state.text);

    // Set texture uniforms
    glUseProgram(state.data->program);
    glUniform1i(glGetUniformLocation(state.data->program, "texture1"), 0);
    glUniform1i(glGetUniformLocation(state.data->program, "texture2"), 1);

    // Initialize camera
    camera_init();

	glEnable(GL_DEPTH_TEST);
	state.id = STATE_PLAYING;
    return 1;
}

void update()
{
    process_input();
    // Game logic here
}

void render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(state.data->program);

    f32 model[16], view[16], proj[16];
    mat4_identity(model);
    mat4_lookat(view, state.cam_pos, vec3_add(state.cam_pos, state.cam_front), state.cam_up);
    mat4_perspective(proj, DEG2RAD(45.0f), (f32)WIDTH / (f32)HEIGHT, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "projection"), 1, GL_FALSE, proj);

    primitive_draw_all(state.prim, state.data->program);
}

void deinit()
{
    texture_registry_cleanup(state.text);
    glfwTerminate();
    primitive_registry_cleanup(state.prim);
    glDeleteProgram(state.data->program);
    free(state.text);
    free(state.prim);
    free(state.data);
    state.text = 0;
    state.prim = 0;
    state.data = 0;
}

i32 main()
{
    ASSERT(init());
    g_lastTime = (f32)glfwGetTime();
    while(!glfwWindowShouldClose(state.win) && state.id != STATE_EXIT)
	{
        const f32 now = (f32)glfwGetTime();
        state.dt = now - g_lastTime;
        g_lastTime = now;

        update();
        render();

        glfwSwapBuffers(state.win);
        glfwPollEvents();
    }

    deinit();
    return 0;
}
