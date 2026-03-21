#include "text.h"
#include "state.h"
#include <stdio.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../ext/stb_image.h"

static texture_registry_t g_registry;

static void texture_set_params(texture_t* tex)
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
    if (g_registry.count >= MAX_TEXTURES) {
        printf("Texture registry full!\n");
        return 0;
    }
    return &g_registry.textures[g_registry.count++];
}

texture_t* texture_create(const char* path, tex_filter_t filter, tex_wrap_t wrap)
{
    texture_t* tex = texture_alloc_slot();
    if (!tex) return &g_registry.fallback;

    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);

    tex->filter = filter;
    tex->wrap_s = wrap;
    tex->wrap_t = wrap;
    tex->has_alpha = false;
    strncpy(tex->name, path, sizeof(tex->name) - 1);
    tex->name[sizeof(tex->name) - 1] = '\0';

    i32 w, h, channels;
    stbi_set_flip_vertically_on_load(1);
    u8* data = stbi_load(path, &w, &h, &channels, 0);

    if (data) {
        tex->width = w;
        tex->height = h;
        tex->channels = channels;
        tex->has_alpha = (channels == 4);

        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(data);
    } else {
        printf("Failed to load texture: %s, using fallback\n", path);
        tex->width = tex->height = 2;
        tex->channels = 3;
        const u8 fallback_px[2 * 2 * 3] = {
            255, 0, 255,  0, 0, 0,
            0, 0, 0,      255, 0, 255
        };
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, fallback_px);
    }

    texture_set_params(tex);
    return tex;
}

texture_t* texture_create_solid(u32 r, u32 g, u32 b)
{
    texture_t* tex = texture_alloc_slot();
    if (!tex) return &g_registry.fallback;

    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);

    tex->filter = TEX_FILTER_LINEAR;
    tex->wrap_s = TEX_WRAP_REPEAT;
    tex->wrap_t = TEX_WRAP_REPEAT;
    tex->width = tex->height = 1;
    tex->channels = 3;
    tex->has_alpha = false;
    strcpy(tex->name, "solid_color");

    const u8 pixel[3] = { (u8)r, (u8)g, (u8)b };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, pixel);
    texture_set_params(tex);
    return tex;
}

texture_t* texture_get_by_name(const char* name)
{
    for (i32 i = 0; i < g_registry.count; i++)
        if (strcmp(g_registry.textures[i].name, name) == 0) return &g_registry.textures[i];
    return 0;
}

void texture_bind(texture_t* tex, u32 unit)
{
    if (!tex) tex = &g_registry.fallback;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, tex->id);
}

void texture_destroy(texture_t* tex)
{
    if (!tex || tex == &g_registry.fallback) return;
    glDeleteTextures(1, &tex->id);
    tex->id = 0;
}

void texture_registry_init(texture_registry_t* reg)
{
    memset(reg, 0, sizeof(*reg));
    reg->count = 0;

    // Create fallback texture
    glGenTextures(1, &reg->fallback.id);
    glBindTexture(GL_TEXTURE_2D, reg->fallback.id);
    reg->fallback.filter = TEX_FILTER_LINEAR;
    reg->fallback.wrap_s = TEX_WRAP_REPEAT;
    reg->fallback.wrap_t = TEX_WRAP_REPEAT;
    reg->fallback.width = reg->fallback.height = 2;
    reg->fallback.channels = 3;
    strcpy(reg->fallback.name, "fallback");

    const u8 fallback_px[2 * 2 * 3] = {
        255, 0, 255,  0, 0, 0,
        0, 0, 0,      255, 0, 255
    };
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, fallback_px);
    glGenerateMipmap(GL_TEXTURE_2D);
    texture_set_params(&reg->fallback);
}

void texture_registry_cleanup(texture_registry_t* reg)
{
    for (i32 i = 0; i < reg->count; i++) {
        glDeleteTextures(1, &reg->textures[i].id);
    }
    glDeleteTextures(1, &reg->fallback.id);
    memset(reg, 0, sizeof(*reg));
}

