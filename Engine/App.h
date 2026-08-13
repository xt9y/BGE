#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdint.h>

#define TITLE "opengl _f"
#define WIDTH 1270
#define HEIGHT 800
#define RENDER_BASE_W 200
#define PALETTE_LEVELS 32.0f

void INPUT(void);
void RENDER(void);
void GL_START(void);
int GL_FRAME(void);
void GL_END(void);
double GL_GETFPS(void);

#include "gfx.h"
#include "state.h"
#include "util/math.h"
#include "util.h"

extern u32 g_fbo;
void fbo_resize(i32 w, i32 h);
void post_blit(i32 src_w, i32 src_h, i32 dst_w, i32 dst_h);
