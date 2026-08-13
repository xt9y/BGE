#include "text.h"
#include "state.h"
#include "util/math.h"

#include <glad/glad.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../Vendor/stb_image.h"

static texture_registry_t* g_registry = NULL;
glyph_uv_t glyphs[128];

static u32 g_text_vao = 0;
static u32 g_text_vbo = 0;
static u32 g_text_program = 0;
static GLint g_text_u_proj = -1;
static GLint g_text_u_font = -1;
static texture_t* g_font_tex = NULL;

static void texture_upload_fallback(texture_t* tex)
{
    static const u8 pixels[12] = {
        180,180,180, 255,255,255,
        255,255,255, 180,180,180
    };
    tex->filter = TEX_FILTER_NEAREST;
    tex->wrap_s = TEX_WRAP_REPEAT;
    tex->wrap_t = TEX_WRAP_REPEAT;
    tex->width = 2;
    tex->height = 2;
    tex->channels = 3;
    tex->has_alpha = false;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
}

static void texture_set_params(const texture_t* tex)
{
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
        tex->wrap_s == TEX_WRAP_REPEAT ? GL_REPEAT :
        tex->wrap_s == TEX_WRAP_CLAMP_TO_EDGE ? GL_CLAMP_TO_EDGE :
        tex->wrap_s == TEX_WRAP_CLAMP_TO_BORDER ? GL_CLAMP_TO_BORDER : GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
        tex->wrap_t == TEX_WRAP_REPEAT ? GL_REPEAT :
        tex->wrap_t == TEX_WRAP_CLAMP_TO_EDGE ? GL_CLAMP_TO_EDGE :
        tex->wrap_t == TEX_WRAP_CLAMP_TO_BORDER ? GL_CLAMP_TO_BORDER : GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        tex->filter == TEX_FILTER_NEAREST ? GL_NEAREST :
        tex->filter == TEX_FILTER_LINEAR ? GL_LINEAR :
        tex->filter == TEX_FILTER_NEAREST_MIPMAP ? GL_NEAREST_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        tex->filter == TEX_FILTER_LINEAR || tex->filter == TEX_FILTER_LINEAR_MIPMAP ? GL_LINEAR : GL_NEAREST);
}

static texture_t* texture_alloc_slot(void)
{
    if (!g_registry) return NULL;
    if (g_registry->count >= MAX_TEXTURES) {
        fprintf(stderr, "BGE: texture registry full (%d textures)\n", MAX_TEXTURES);
        return NULL;
    }
    texture_t* tex = &g_registry->textures[g_registry->count++];
    memset(tex, 0, sizeof(*tex));
    return tex;
}

bool texture_registry_init(texture_registry_t* reg)
{
    if (!reg || g_registry) return false;
    memset(reg, 0, sizeof(*reg));
    g_registry = reg;

    glGenTextures(1, &reg->fallback.id);
    if (!reg->fallback.id) {
        g_registry = NULL;
        return false;
    }
    snprintf(reg->fallback.name, sizeof(reg->fallback.name), "%s", "fallback");
    glBindTexture(GL_TEXTURE_2D, reg->fallback.id);
    texture_upload_fallback(&reg->fallback);
    texture_set_params(&reg->fallback);
    return true;
}

void texture_registry_cleanup(texture_registry_t* reg)
{
    if (!reg) return;
    for (i32 i = 0; i < reg->count; ++i)
        if (reg->textures[i].id) glDeleteTextures(1, &reg->textures[i].id);
    if (reg->fallback.id) glDeleteTextures(1, &reg->fallback.id);
    if (g_registry == reg) g_registry = NULL;
    memset(reg, 0, sizeof(*reg));
}

texture_t* texture_create(const char* path, const tex_filter_t filter, const tex_wrap_t wrap)
{
    if (!path || !path[0]) return NULL;
    texture_t* tex = texture_alloc_slot();
    if (!tex) return g_registry ? &g_registry->fallback : NULL;

    glGenTextures(1, &tex->id);
    if (!tex->id) {
        --g_registry->count;
        return &g_registry->fallback;
    }
    glBindTexture(GL_TEXTURE_2D, tex->id);

    tex->filter = filter;
    tex->wrap_s = wrap;
    tex->wrap_t = wrap;
    snprintf(tex->name, sizeof(tex->name), "%s", path);

    i32 w = 0, h = 0, channels = 0;
    stbi_set_flip_vertically_on_load(0);
    u8* pixels = stbi_load(path, &w, &h, &channels, 0);
    if (!pixels || (channels != 3 && channels != 4)) {
        if (pixels) stbi_image_free(pixels);
        fprintf(stderr, "BGE: failed to load texture '%s'; using fallback pixels\n", path);
        texture_upload_fallback(tex);
        texture_set_params(tex);
        return tex;
    }

    tex->width = w;
    tex->height = h;
    tex->channels = channels;
    tex->has_alpha = channels == 4;
    const GLenum format = channels == 4 ? GL_RGBA : GL_RGB;
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, pixels);
    if (filter == TEX_FILTER_NEAREST_MIPMAP || filter == TEX_FILTER_LINEAR_MIPMAP)
        glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(pixels);
    texture_set_params(tex);
    return tex;
}

texture_t* texture_create_solid(const u32 r, const u32 g, const u32 b)
{
    texture_t* tex = texture_alloc_slot();
    if (!tex) return g_registry ? &g_registry->fallback : NULL;

    glGenTextures(1, &tex->id);
    if (!tex->id) {
        --g_registry->count;
        return &g_registry->fallback;
    }

    tex->filter = TEX_FILTER_LINEAR;
    tex->wrap_s = TEX_WRAP_REPEAT;
    tex->wrap_t = TEX_WRAP_REPEAT;
    tex->width = 1;
    tex->height = 1;
    tex->channels = 3;
    tex->has_alpha = false;
    snprintf(tex->name, sizeof(tex->name), "%s", "solid_color");

    const u8 pixel[3] = {(u8)r, (u8)g, (u8)b};
    glBindTexture(GL_TEXTURE_2D, tex->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    texture_set_params(tex);
    return tex;
}

texture_t* texture_get_by_name(const char* name)
{
    if (!g_registry || !name) return NULL;
    for (i32 i = 0; i < g_registry->count; ++i)
        if (strcmp(g_registry->textures[i].name, name) == 0)
            return &g_registry->textures[i];
    return NULL;
}

void texture_bind(const texture_t* tex, const u32 unit)
{
    if (!g_registry) return;
    if (!tex || !tex->id) tex = &g_registry->fallback;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, tex->id);
}

void texture_destroy(texture_t* tex)
{
    if (!g_registry || !tex || tex == &g_registry->fallback || !tex->id) return;
    glDeleteTextures(1, &tex->id);
    tex->id = 0;
}

const texture_t* texture_get_fallback(void)
{
    return g_registry ? &g_registry->fallback : NULL;
}

static void text_init_glyphs(void)
{
    memset(glyphs, 0, sizeof(glyphs));
    glyphs[' '] = (glyph_uv_t){0, 2};
    glyphs['('] = (glyph_uv_t){8, 2}; glyphs[')'] = (glyph_uv_t){9, 2};
    glyphs['*'] = (glyph_uv_t){10, 2}; glyphs['+'] = (glyph_uv_t){11, 2};
    glyphs['-'] = (glyph_uv_t){13, 2}; glyphs['.'] = (glyph_uv_t){14, 2}; glyphs['/'] = (glyph_uv_t){15, 2};
    glyphs['0'] = (glyph_uv_t){0,3}; glyphs['1'] = (glyph_uv_t){1,3}; glyphs['2'] = (glyph_uv_t){2,3}; glyphs['3'] = (glyph_uv_t){3,3};
    glyphs['4'] = (glyph_uv_t){4,3}; glyphs['5'] = (glyph_uv_t){5,3}; glyphs['6'] = (glyph_uv_t){6,3}; glyphs['7'] = (glyph_uv_t){7,3};
    glyphs['8'] = (glyph_uv_t){8,3}; glyphs['9'] = (glyph_uv_t){9,3}; glyphs[':'] = (glyph_uv_t){10,3}; glyphs[';'] = (glyph_uv_t){11,3};
    glyphs['<'] = (glyph_uv_t){12,3}; glyphs['='] = (glyph_uv_t){13,3}; glyphs['>'] = (glyph_uv_t){14,3}; glyphs['?'] = (glyph_uv_t){15,3};
    for (i32 i = 0; i < 15; ++i) glyphs['A' + i] = (glyph_uv_t){(u8)(i + 1), 4};
    glyphs['P'] = (glyph_uv_t){0,5};
    for (i32 i = 1; i < 11; ++i) glyphs['P' + i] = (glyph_uv_t){(u8)i, 5};
    glyphs['_'] = (glyph_uv_t){15,5};
    for (i32 i = 0; i < 15; ++i) glyphs['a' + i] = (glyph_uv_t){(u8)(i + 1), 6};
    glyphs['p'] = (glyph_uv_t){0,7};
    for (i32 i = 1; i < 11; ++i) glyphs['p' + i] = (glyph_uv_t){(u8)i, 7};
}

static void text_ortho(f32* m, const f32 left, const f32 right, const f32 bottom, const f32 top)
{
    const f32 rl = right - left;
    const f32 tb = top - bottom;
    mat4_identity(m);
    m[0] = 2.0f / rl;
    m[5] = 2.0f / tb;
    m[10] = -1.0f;
    m[12] = -(right + left) / rl;
    m[13] = -(top + bottom) / tb;
}

bool text_init(void)
{
    if (!g_registry || g_text_program || g_text_vao || g_text_vbo) return false;
    text_init_glyphs();

    g_font_tex = texture_create("Engine/res/font.png", TEX_FILTER_NEAREST, TEX_WRAP_CLAMP_TO_EDGE);
    if (!g_font_tex) return false;

    static const char* vs =
        "#version 330 core\n"
        "layout(location=0) in vec3 aPos;\n"
        "layout(location=1) in vec2 aUV;\n"
        "layout(location=2) in vec4 aColor;\n"
        "out vec2 vUV; out vec4 vColor;\n"
        "uniform mat4 u_proj;\n"
        "void main(){ vUV=aUV; vColor=aColor; gl_Position=u_proj*vec4(aPos,1.0); }\n";
    static const char* fs =
        "#version 330 core\n"
        "in vec2 vUV; in vec4 vColor; out vec4 FragColor;\n"
        "uniform sampler2D u_font;\n"
        "void main(){ FragColor=texture(u_font,vUV)*vColor; }\n";

    g_text_program = create_program(vs, fs);
    if (!g_text_program) { g_font_tex = NULL; return false; }
    g_text_u_proj = glGetUniformLocation(g_text_program, "u_proj");
    g_text_u_font = glGetUniformLocation(g_text_program, "u_font");
    if (g_text_u_proj < 0 || g_text_u_font < 0) {
        text_shutdown();
        return false;
    }

    glGenVertexArrays(1, &g_text_vao);
    glGenBuffers(1, &g_text_vbo);
    if (!g_text_vao || !g_text_vbo) {
        text_shutdown();
        return false;
    }

    glBindVertexArray(g_text_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_text_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * MAX_TEXT_VERTICES, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(f32) * 3));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)(sizeof(f32) * 5));
    glBindVertexArray(0);
    return true;
}

void text_shutdown(void)
{
    if (g_text_vbo) glDeleteBuffers(1, &g_text_vbo);
    if (g_text_vao) glDeleteVertexArrays(1, &g_text_vao);
    if (g_text_program) glDeleteProgram(g_text_program);
    g_text_vbo = 0;
    g_text_vao = 0;
    g_text_program = 0;
    g_text_u_proj = -1;
    g_text_u_font = -1;
    g_font_tex = NULL;
}

u32 text_get_program(void) { return g_text_program; }
GLint text_projection_uniform(void) { return g_text_u_proj; }
GLint text_sampler_uniform(void) { return g_text_u_font; }

void text_begin(void)
{
    if (g_state) state.text_vertex_count = 0;
}

static void draw_char(const u8 c, const f32 x, const f32 y, const f32 width, const f32 height)
{
    if (!g_state || c >= 128 || state.text_vertex_count + 6 > MAX_TEXT_VERTICES) return;
    const f32 u0 = (f32)glyphs[c].x / 16.0f;
    const f32 v0 = (f32)glyphs[c].y / 16.0f;
    const f32 u1 = u0 + 1.0f / 16.0f;
    const f32 v1 = v0 + 1.0f / 16.0f;
    const f32 z = 0.0f;
    const vec4s white = {1,1,1,1};

    state.text_vertices[state.text_vertex_count++] = (vertex_t){{x,y,z},{u0,v0},white};
    state.text_vertices[state.text_vertex_count++] = (vertex_t){{x+width,y+height,z},{u1,v1},white};
    state.text_vertices[state.text_vertex_count++] = (vertex_t){{x+width,y,z},{u1,v0},white};
    state.text_vertices[state.text_vertex_count++] = (vertex_t){{x,y,z},{u0,v0},white};
    state.text_vertices[state.text_vertex_count++] = (vertex_t){{x,y+height,z},{u0,v1},white};
    state.text_vertices[state.text_vertex_count++] = (vertex_t){{x+width,y+height,z},{u1,v1},white};
}

void text_draw(const vec2s pos, const char* fmt, ...)
{
    if (!fmt) return;
    char buffer[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    f32 x = pos.x;
    const unsigned char* p = (const unsigned char*)buffer;
    while (*p) {
        u8 c = *p;
        if (c < 128) {
            draw_char(c, x, pos.y, CHAR_WIDTH, CHAR_HEIGHT);
            ++p;
        } else {
            /* Font atlas is ASCII-only. Consume one UTF-8 codepoint safely. */
            draw_char((u8)'?', x, pos.y, CHAR_WIDTH, CHAR_HEIGHT);
            ++p;
            while ((*p & 0xC0u) == 0x80u) ++p;
        }
        x += CHAR_WIDTH * CHAR_SPACING;
    }
}

void text_flush(const i32 fbw, const i32 fbh)
{
    if (!g_state || state.text_vertex_count == 0 || !g_font_tex || !g_text_program) return;
    if (fbw <= 0 || fbh <= 0) return;

    f32 proj[16];
    text_ortho(proj, 0.0f, (f32)fbw, (f32)fbh, 0.0f);

    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(g_text_program);
    glUniformMatrix4fv(g_text_u_proj, 1, GL_FALSE, proj);
    glUniform1i(g_text_u_font, 0);
    texture_bind(g_font_tex, 0);

    glBindVertexArray(g_text_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_text_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_t) * state.text_vertex_count, state.text_vertices);
    glDrawArrays(GL_TRIANGLES, 0, (GLsizei)state.text_vertex_count);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}
