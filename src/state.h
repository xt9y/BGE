#ifndef STATE_H
#define STATE_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "text.h"
#include "primv.h"
#include "cam.h"
#include "util/types.h"

#define ASSERT(x) do { if (!(x)) { printf("Assertion failed: %s\n", #x); exit(1); } } while(0)
#define LOG(x) do { printf("%s\n", x); } while(0)

typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_EXIT
} state_e;

typedef struct {
    u32 vao, vbo, ebo, program;
} data_t;

#ifdef MAIN
#define MAIN
typedef struct {
    GLFWwindow* win;
    state_e id;
    f32 dt;
    data_t* data;
    camera_t cam;
    texture_registry_t* text;
    primitive_registry_t* prim;
} state_t;

extern state_t state;
#endif

#endif
