#ifndef BGE_GFX_H
#define BGE_GFX_H

#include <glad/glad.h>
#include "util/types.h"

u32 compile_shader(u32 type, const char* src);
u32 create_program(const char* vs, const char* fs);

#define VS "#version 330 core\n" \
    "layout(location=0) in vec3 aPos;\n" \
    "layout(location=1) in vec3 aColor;\n" \
    "layout(location=2) in vec2 aTexCoord;\n" \
    "out vec3 ourColor;\n" \
    "out vec2 TexCoord;\n" \
    "uniform mat4 model;\n" \
    "uniform mat4 view;\n" \
    "uniform mat4 projection;\n" \
    "uniform vec3 u_tint;\n" \
    "uniform vec2 u_uv_scale;\n" \
    "void main(){\n" \
    "    gl_Position=projection * view * model * vec4(aPos,1.0);\n" \
    "    ourColor=aColor * u_tint;\n" \
    "    TexCoord=aTexCoord * u_uv_scale;\n" \
    "}"

#define FS "#version 330 core\n" \
    "out vec4 FragColor;\n" \
    "in vec3 ourColor;\n" \
    "in vec2 TexCoord;\n" \
    "uniform sampler2D texture1;\n" \
    "void main(){\n" \
    "    vec4 texColor=texture(texture1,TexCoord);\n" \
    "    FragColor=vec4(texColor.rgb*ourColor,texColor.a);\n" \
    "}"

#endif
