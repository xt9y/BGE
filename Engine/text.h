#ifndef TEXTURE_H
#define TEXTURE_H

#include "gfx.h"
#include "util/types.h"

#define MAX_TEXTURES 16
#define MAX_TEXT_VERTICES 4096

#define CHAR_WIDTH 8.0f
#define CHAR_HEIGHT 8.0f
#define CHAR_SPACING 1.0f

typedef enum {
    TEX_FILTER_NEAREST = 0,
    TEX_FILTER_LINEAR,
    TEX_FILTER_NEAREST_MIPMAP,
    TEX_FILTER_LINEAR_MIPMAP
} tex_filter_t;

typedef enum {
    TEX_WRAP_REPEAT = 0,
    TEX_WRAP_CLAMP_TO_EDGE,
    TEX_WRAP_CLAMP_TO_BORDER,
    TEX_WRAP_MIRRORED_REPEAT
} tex_wrap_t;

typedef struct {
    u32 id;
    i32 width, height, channels;
    tex_filter_t filter;
    tex_wrap_t wrap_s;
    tex_wrap_t wrap_t;
    bool has_alpha;
    char name[64];
} texture_t;

typedef struct {
    texture_t textures[MAX_TEXTURES];
    i32 count;
    texture_t fallback;
} texture_registry_t;

typedef struct {
    vec3s pos;
    vec2s uv;
    vec4s color;
} vertex_t;

typedef struct
{
    u8 x;
    u8 y;
} glyph_uv_t;

extern glyph_uv_t glyphs[128];

texture_t* texture_create(const char* path, tex_filter_t filter, tex_wrap_t wrap);
texture_t* texture_create_solid(u32 r, u32 g, u32 b);
texture_t* texture_get_by_name(const char* name);
void texture_bind(texture_t* tex, u32 unit);
void texture_destroy(texture_t* tex);
void texture_registry_init(texture_registry_t* reg);
void texture_registry_cleanup(texture_registry_t* reg);

const texture_t* texture_get_fallback(void);

void text_init(texture_t* font_tex);
void text_shutdown(void);
void text_begin(void);
void text_draw_string(const char* str, float x, float y);
void text_draw_textf(float x, float y, const char* fmt, ...);
void text_flush(int fbw, int fbh);

#define VK_BEGINTEXT do { text_begin(); } while (0)
#define VK_DRAWTEXT(x, y, str) text_draw_string((str), (x), (y))
#define VK_DRAWTEXTF(x, y, fmt, ...) text_draw_textf((x), (y), (fmt), __VA_ARGS__)

#endif
