#include "gfx.h"
#include <glad/glad.h>
#include <stdio.h>

u32 compile_shader(const u32 type, const char* src)
{
    if (!src) return 0;

    const u32 shader = glCreateShader(type);
    if (!shader) return 0;

    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    i32 ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[2048] = {0};
        glGetShaderInfoLog(shader, (GLsizei)sizeof(log), NULL, log);
        fprintf(stderr, "BGE: shader compilation failed: %s\n", log);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

u32 create_program(const char* vs, const char* fs)
{
    const u32 vertex = compile_shader(GL_VERTEX_SHADER, vs);
    if (!vertex) return 0;

    const u32 fragment = compile_shader(GL_FRAGMENT_SHADER, fs);
    if (!fragment) {
        glDeleteShader(vertex);
        return 0;
    }

    const u32 program = glCreateProgram();
    if (!program) {
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return 0;
    }

    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    i32 ok = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[2048] = {0};
        glGetProgramInfoLog(program, (GLsizei)sizeof(log), NULL, log);
        fprintf(stderr, "BGE: shader program link failed: %s\n", log);
        glDeleteProgram(program);
        return 0;
    }

    return program;
}
