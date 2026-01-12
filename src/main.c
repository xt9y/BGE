#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include "state.h"
#include "gfx.h"

#define WIDTH 800
#define HEIGHT 600

State state;   // GLOBAL GAME STATE
float g_lastTime;

void framebuffer_size_callback(GLFWwindow* w,int width,int height)
{ 
    (void)w; glViewport(0,0,width,height); 
}

void process_input()
{ 
    if(glfwGetKey(state.win , GLFW_KEY_ESCAPE) == GLFW_PRESS) state.id = STATE_EXIT; 
}

#define VS "#version 330 core\nlayout(location=0) in vec3 aPos;void main(){gl_Position=vec4(aPos,1);}"
#define FS "#version 330 core\nout vec4 C;void main(){C=vec4(1,.5,.2,1);} "

void init()
{
    PlayingData* d = malloc(sizeof(*d));
    state.data = d;

    float v[] = {-.5f,-.5f,0,  .5f,-.5f,0,  0,.5f,0};

    glGenVertexArrays(1, &state.data->vao);
    glGenBuffers(1, &state.data->vbo);

    glBindVertexArray(state.data->vao);
    glBindBuffer(GL_ARRAY_BUFFER, state.data->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);

    glVertexAttribPointer(0,3,GL_FLOAT,0,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    d->program = create_program(VS,FS);
}

void update()
{
    // use state.dt, state.win, etc directly
}

void render()
{
    glClearColor(.2f,.3f,.3f,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(state.data->program);
    glBindVertexArray(state.data->vao);
    glDrawArrays(GL_TRIANGLES,0,3);
}

void deinit()
{
    glDeleteVertexArrays(1, &state.data->vao);
    glDeleteBuffers(1, &state.data->vbo);
    glDeleteProgram(state.data->program);
    free(state.data);
    state.data = 0;
}

int main()
{
    if(!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    state.win = glfwCreateWindow(WIDTH, HEIGHT, "GAME", 0, 0);
    if(!state.win) { glfwTerminate(); return 1; }

    glfwMakeContextCurrent(state.win);
    glfwSetFramebufferSizeCallback(state.win, framebuffer_size_callback);

    g_lastTime = (float)glfwGetTime();

    init();
    state.id = STATE_MENU;
    state.id = STATE_PLAYING;

    while(!glfwWindowShouldClose(state.win) && state.id! = STATE_EXIT){
        float now = (float)glfwGetTime();
        state.dt = now-g_lastTime; g_lastTime = now;
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

