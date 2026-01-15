#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "state.h"
#include "gfx.h"
#include "text.h"
#include "util/math.h"
#include "cam.h"

#define WIDTH 800
#define HEIGHT 600

f32 g_lastTime;

void mouse_callback(GLFWwindow* window, f64 xpos, f64 ypos)
{
    camera_mouse_callback(xpos, ypos);
}

void framebuffer_size_callback(GLFWwindow* w, i32 width, i32 height)
{
    glViewport(0, 0, width, height);
}

#define process_input() do { \
    if(glfwGetKey(state.win, GLFW_KEY_ESCAPE) == GLFW_PRESS) state.id=STATE_EXIT; \
    update_camera(); \
} while (0)

// Updated shaders with MVP support
#define VS "#version 330 core\n" \
    "layout(location=0) in vec3 aPos;\n" \
    "layout(location=1) in vec3 aColor;\n" \
    "layout(location=2) in vec2 aTexCoord;\n" \
    "out vec3 ourColor;\n" \
    "out vec2 TexCoord;\n" \
    "uniform mat4 model;\n" \
    "uniform mat4 view;\n" \
    "uniform mat4 projection;\n" \
    "void main(){\n" \
    "    gl_Position=projection * view * model * vec4(aPos,1.0);\n" \
    "    ourColor=aColor;\n" \
    "    TexCoord=aTexCoord;\n" \
    "}"

#define FS "#version 330 core\n" \
    "out vec4 FragColor;\n" \
    "in vec3 ourColor;\n" \
    "in vec2 TexCoord;\n" \
    "uniform sampler2D texture1;\n" \
    "uniform sampler2D texture2;\n" \
    "void main(){\n" \
    "    FragColor=mix(texture(texture1,TexCoord),texture(texture2,TexCoord),0.2);\n" \
    "}"

void init()
{
    state.data = malloc(sizeof(*state.data));
    state.text = malloc(sizeof(*state.text));
    
    // Vertex data with positions, colors, and texture coords
    f32 vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f   // top left
    };
    
    u32 indices[] = {
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
    
    // Initialize textures
    init_texture();
    
    // Set texture uniforms
    glUseProgram(state.data->program);
    glUniform1i(glGetUniformLocation(state.data->program, "texture1"), 0);
    glUniform1i(glGetUniformLocation(state.data->program, "texture2"), 1);

    // Initialize camera
    init_camera();

    glEnable(GL_DEPTH_TEST);
}

void update()
{
    // Game logic here
}

void render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Bind textures
    update_texture();
    
    // Render quad
    glUseProgram(state.data->program);

    f32 model[16], view[16], proj[16];
    mat4_identity(model);
    mat4_lookat(view, state.cam_pos, vec3_add(state.cam_pos, state.cam_front), state.cam_up);
    mat4_perspective(proj, 45.0f * 3.1415926535f / 180.0f, (f32)WIDTH/(f32)HEIGHT, 0.1f, 100.0f);
    
    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "model"), 1, GL_FALSE, model);
    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "view"), 1, GL_FALSE, view);
    glUniformMatrix4fv(glGetUniformLocation(state.data->program, "projection"), 1, GL_FALSE, proj);

    glBindVertexArray(state.data->vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void deinit()
{
    deinit_texture();
    glDeleteVertexArrays(1, &state.data->vao);
    glDeleteBuffers(1, &state.data->vbo);
    glDeleteBuffers(1, &state.data->ebo);
    glDeleteProgram(state.data->program);
    free(state.text);
    free(state.data);
    state.text = 0;
    state.data = 0;
}

int main()
{
    if(!glfwInit()) return 1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    state.win = glfwCreateWindow(WIDTH, HEIGHT, "Texture Demo", 0, 0);
    if(!state.win) { glfwTerminate(); return 1; }
    
    glfwMakeContextCurrent(state.win);
    glfwSetFramebufferSizeCallback(state.win, framebuffer_size_callback);
    glfwSetCursorPosCallback(state.win, mouse_callback);
    glfwSetInputMode(state.win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    g_lastTime = (f32)glfwGetTime();
    
    init();
    state.id = STATE_PLAYING;
    
    while(!glfwWindowShouldClose(state.win) && state.id != STATE_EXIT)
	{
        f32 now = (f32)glfwGetTime();
        state.dt = now - g_lastTime;
        g_lastTime = now;
        
        process_input();
        update();
        render();
        
        glfwSwapBuffers(state.win);
        glfwPollEvents();
    }
    
    deinit();
    glfwTerminate();
    return 0;
}

