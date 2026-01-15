#ifndef TEXTURE_H
#define TEXTURE_H

#include "gfx.h"

typedef struct {
    u32 t1, t2, def;
    u8 *data;  // Fixed: should be pointer
    i32 width, height, nrChannels;
} texture_t;

void init_texture();
void update_texture();
void deinit_texture();

#endif

