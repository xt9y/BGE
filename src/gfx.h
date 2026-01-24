#ifndef GFX_H
#define GFX_H
#include <OpenGL/gl3.h>
#include "util/types.h"

u32 compile_shader(u32 type, const char* src);
u32 create_program(const char* vs, const char* fs);

// Updated shaders with MVP support
#define VS "#version 330 core\n" \
    "layout(location=0) in vec3 aPos;\n" \
    "layout(location=1) in vec3 aColor;\n" \
    "layout(location=2) in vec2 aTexCoord;\n" \
    "out vec3 ourColor;\n" \
    "out vec2 TexCoord;\n" \
    "uniform mat4 model;\n" \
    "uniform mat4 view;\n" \
    "uniform mat4 projection;\n" \
    "void main(){\n" \
    "    gl_Position=projection * view * model * vec4(aPos,1.0);\n" \
    "    ourColor=aColor;\n" \
    "    TexCoord=aTexCoord;\n" \
    "}"

#define FS "#version 330 core\n" \
    "out vec4 FragColor;\n" \
    "in vec3 ourColor;\n" \
    "in vec2 TexCoord;\n" \
    "uniform sampler2D texture1;\n" \
    "uniform sampler2D texture2;\n" \
    "void main(){\n" \
    "    FragColor=mix(texture(texture1,TexCoord),texture(texture2,TexCoord),0.2);\n" \
    "}"

#endif

