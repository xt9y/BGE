#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Engine Application API
// Simple interface for creating OpenGL applications

// Main macro
// and the engine handles the rest
void INPUT();
void RENDER();
void RUN(int argc, char** argv);
#define ENGINE_ENTRY_POINT \
    int main(int argc, char** argv) { \
        RUN(argc, argv); \
        return 0; \
    }

#define TITLE  "opengl _f"
#define WIDTH 1270 
#define HEIGHT 800
#define RENDER_BASE_W 200
#define PALETTE_LEVELS 32.0f

void GL_START();
int  GL_FRAME();
void GL_END();
double GL_GETFPS();

#include "Engine/gfx.h"
#include "Engine/state.h"
#include "Engine/util/math.h"
#include "Engine/util.h"

extern u32 g_fbo;
void fbo_resize(const i32 w, const i32 h);
void post_blit(i32 src_w, i32 src_h, i32 dst_w, i32 dst_h);
