#include "text.h"
#include "state.h"
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../ext/stb_image.h"

void init_texture()
{
    // Texture 1
    glGenTextures(1, &state.text->t1);
    glBindTexture(GL_TEXTURE_2D, state.text->t1);  // Fixed typo
    // Texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load first image
    stbi_set_flip_vertically_on_load(1);
    state.text->data = stbi_load("res/stone.png", &state.text->width, &state.text->height, &state.text->nrChannels, 0);
    
    if (state.text->data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, state.text->width, state.text->height, 0, GL_RGB, GL_UNSIGNED_BYTE, state.text->data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(state.text->data);
    } else {
        printf("Failed to load texture 1\n");
    }
    
    // Texture 2
    glGenTextures(1, &state.text->t2);
    glBindTexture(GL_TEXTURE_2D, state.text->t2);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    state.text->data = stbi_load("res/awesomeface.png", &state.text->width, &state.text->height, &state.text->nrChannels, 0);
    
    if (state.text->data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, state.text->width, state.text->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, state.text->data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(state.text->data);
    } else {
        printf("Failed to load texture 2\n");
    }
}

void update_texture()
{
    // Bind textures before rendering
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state.text->t1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, state.text->t2);
}

void deinit_texture()
{
    glDeleteTextures(1, &state.text->t1);
    glDeleteTextures(1, &state.text->t2);
}

