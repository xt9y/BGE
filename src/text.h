#ifndef TEXTURE_H
#define TEXTURE_H

#include "gfx.h"

typedef struct {
    unsigned int t1, t2;
    unsigned char *data;  // Fixed: should be pointer
    int width, height, nrChannels;
} texture_t;

void init_texture();
void update_texture();
void deinit_texture();

#endif

