#ifndef TEXTURE_H
#define TEXTURE_H

#include "gfx.h"

typedef struct {
    u32 t1, t2, t3, def;
    u8 *data;
    i32 width, height, nrChannels;
} texture_t;

void init_texture();
void update_texture();
void deinit_texture();

#endif
