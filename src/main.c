#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "state.h"
#include "gfx.h"
#include "text.h"

#define WIDTH 800
#define HEIGHT 600

float g_lastTime;

#define process_input() do { \
    if(glfwGetKey(state.win,GLFW_KEY_ESCAPE)==GLFW_PRESS) state.id=STATE_EXIT; \
} while(0)

// Updated shaders with texture support
#define VS "#version 330 core\n" \
    "layout(location=0) in vec3 aPos;\n" \
    "layout(location=1) in vec3 aColor;\n" \
    "layout(location=2) in vec2 aTexCoord;\n" \
    "out vec3 ourColor;\n" \
    "out vec2 TexCoord;\n" \
    "void main(){\n" \
    "    gl_Position=vec4(aPos,1.0);\n" \
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

void framebuffer_size_callback(GLFWwindow* w, int width, int height)
{
    glViewport(0, 0, width, height);
}

void init()
{
    state.data = malloc(sizeof(*state.data));
    state.text = malloc(sizeof(*state.text));
    
    // Vertex data with positions, colors, and texture coords
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f   // top left
    };
    
    unsigned int indices[] = {
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    state.data->program = create_program(VS, FS);
    
    // Initialize textures
    init_texture();
    
    // Set texture uniforms
    glUseProgram(state.data->program);
    glUniform1i(glGetUniformLocation(state.data->program, "texture1"), 0);
    glUniform1i(glGetUniformLocation(state.data->program, "texture2"), 1);
}

void update()
{
    // Game logic here
}

void render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Bind textures
    update_texture();
    
    // Render quad
    glUseProgram(state.data->program);
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
    
    g_lastTime = (float)glfwGetTime();
    
    init();
    state.id = STATE_PLAYING;
    
    while(!glfwWindowShouldClose(state.win) && state.id != STATE_EXIT) {
        float now = (float)glfwGetTime();
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

