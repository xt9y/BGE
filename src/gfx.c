#include "gfx.h"
#include <stdio.h>

u32 compile_shader(const u32 type, const char* src)
{
    const u32 s = glCreateShader(type);
    glShaderSource(s,1,&src,0);
    glCompileShader(s);
    i32 ok; glGetShaderiv(s,GL_COMPILE_STATUS,&ok);
    if (!ok) { 
        char log[512]; 
        glGetShaderInfoLog(s,512,0,log); 
        printf("Shader error: %s\n",log);
    }    
    return s;
}

u32 create_program(const char* vs, const char* fs)
{
    const u32 v = compile_shader(GL_VERTEX_SHADER,vs);
    const u32 f = compile_shader(GL_FRAGMENT_SHADER,fs);
    const u32 p = glCreateProgram();
    glAttachShader(p,v); glAttachShader(p,f); glLinkProgram(p);
    glDeleteShader(v); glDeleteShader(f);
    return p;
}

