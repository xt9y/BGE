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
    update_camera(); \
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
    state.text = malloc(sizeof(*state.text));
    memset(state.text, 0, sizeof(*state.text));

    // Vertex data with positions, colors, and texture coords
    // Two triangles sharing one texture
    const f32 vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f   // top left
    };

    const u32 indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };

    glGenVertexArrays(1, &state.data->vao);
    glGenBuffers(1, &state.data->vbo);
    glGenBuffers(1, &state.data->ebo);

    glBindVertexArray(state.data->vao);

    glBindBuffer(GL_ARRAY_BUFFER, state.data->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state.data->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(3 * sizeof(f32)));
    glEnableVertexAttribArray(1);

    // Texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(f32), (void*)(6 * sizeof(f32)));
    glEnableVertexAttribArray(2);

    state.data->program = create_program(VS, FS);

    // Initialize texture registry
    texture_registry_init(state.text);

    // Create textures 
    state.text->textures[0] = *texture_create("res/ground.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    state.text->textures[1] = *texture_create("res/stone.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);
    state.text->textures[2] = *texture_create("res/awesomeface.png", TEX_FILTER_LINEAR, TEX_WRAP_REPEAT);

    // Set texture uniforms
    glUseProgram(state.data->program);
    glUniform1i(glGetUniformLocation(state.data->program, "texture1"), 0);
    glUniform1i(glGetUniformLocation(state.data->program, "texture2"), 1);

    // Initialize camera
    init_camera();

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

    // Bind textures two triangles share texture1
    texture_bind(&state.text->textures[0], 0);
    texture_bind(&state.text->textures[2], 1);

    // Render quad 
    glUseProgram(state.data->program);

    f32 model[16], view[16], proj[16];
    mat4_identity(model);
    mat4_lookat(view, state.cam_pos, vec3_add(state.cam_pos, state.cam_front), state.cam_up);
    mat4_perspective(proj, DEG2RAD(45.0f), (f32)WIDTH / (f32)HEIGHT, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "model"), 1, GL_FALSE, model);
    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "projection"), 1, GL_FALSE, proj);

    glBindVertexArray(state.data->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void deinit()
{
    texture_registry_cleanup(state.text);
    glfwTerminate();
    glDeleteVertexArrays(1, &state.data->vao);
    glDeleteBuffers(1, &state.data->vbo);
    glDeleteBuffers(1, &state.data->ebo);
    glDeleteProgram(state.data->program);
    free(state.text);
    free(state.data);
    state.text = 0;
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
