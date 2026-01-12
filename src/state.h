#ifndef STATE_H
#define STATE_H
#include <GLFW/glfw3.h>

typedef enum { 
    STATE_MENU, 
    STATE_PLAYING, 
    STATE_EXIT 
} StateID;

typedef struct { 
    unsigned int vao,
                 vbo,
                 program; 
} PlayingData;

typedef struct {
    StateID id;
    GLFWwindow* win;
    float dt;
    PlayingData* data;
} State;

extern State state;
void set_state(StateID id);

#endif


