#ifndef GFX_H
#define GFX_H
#include <OpenGL/gl3.h>
#include "util/types.h"

u32 compile_shader(u32 type, const char* src);
u32 create_program(const char* vs, const char* fs);

#endif

