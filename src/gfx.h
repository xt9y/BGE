#ifndef GFX_H
#define GFX_H
#include <OpenGL/gl3.h>

unsigned int compile_shader(unsigned int type, const char* src);
unsigned int create_program(const char* vs, const char* fs);

#endif

