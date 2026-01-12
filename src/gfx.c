#include "gfx.h"
#include <stdio.h>

unsigned int compile_shader(unsigned int type, const char* src)
{
    unsigned int s = glCreateShader(type);
    glShaderSource(s,1,&src,0);
    glCompileShader(s);
    int ok; glGetShaderiv(s,GL_COMPILE_STATUS,&ok);
    if(!ok){ char log[512]; glGetShaderInfoLog(s,512,0,log); printf("Shader error: %s\n",log);}    
    return s;
}

unsigned int create_program(const char* vs, const char* fs)
{
    unsigned int v=compile_shader(GL_VERTEX_SHADER,vs);
    unsigned int f=compile_shader(GL_FRAGMENT_SHADER,fs);
    unsigned int p=glCreateProgram();
    glAttachShader(p,v); glAttachShader(p,f); glLinkProgram(p);
    glDeleteShader(v); glDeleteShader(f);
    return p;
}

