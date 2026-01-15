#ifndef STATE_H
#define STATE_H

#include <GLFW/glfw3.h>
#include "text.h"

#define ASSERT(x) do { \
    if(!(x)) { printf("Assertion failed: %s\n", #x); exit(1); } \
} while(0)

typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_EXIT
} state_e;

typedef struct {
    unsigned int vao, vbo, ebo, program;  // Added ebo
} data_t;

typedef struct {
    GLFWwindow* win;
    state_e id;
    float dt;
    data_t* data;
    texture_t* text;
} state_t;

state_t state;

#endif

