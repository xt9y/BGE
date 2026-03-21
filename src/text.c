#include "text.h"
#include "state.h"
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../ext/stb_image.h"

void init_texture()
{
	// Flip textures vertically
	stbi_set_flip_vertically_on_load(1);

    // Fallback texture (pink/black test texture, 2x2)
    glGenTextures(1, &state.text->def);
    glBindTexture(GL_TEXTURE_2D, state.text->def);
    // Texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    {
        const u8 fallback_px[2 * 2 * 3] = {
            255, 0, 255,  0, 0, 0,
            0, 0, 0,      255, 0, 255
        };
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_UNSIGNED_BYTE, fallback_px);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Texture 3
    glGenTextures(1, &state.text->t3);
    glBindTexture(GL_TEXTURE_2D, state.text->t3);
    // Texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image
    state.text->data = stbi_load("res/stone.png", &state.text->width, &state.text->height, &state.text->nrChannels, 0);
    if (state.text->data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, state.text->width, state.text->height, 0, GL_RGB, GL_UNSIGNED_BYTE, state.text->data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(state.text->data);
    } else {
        glBindTexture(GL_TEXTURE_2D, state.text->def);
        state.text->t3 = state.text->def;
    }

    // Texture 1
    glGenTextures(1, &state.text->t1);
    glBindTexture(GL_TEXTURE_2D, state.text->t1);
    // Texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image
    state.text->data = stbi_load("res/ground.png", &state.text->width, &state.text->height, &state.text->nrChannels, 0);
    if (state.text->data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, state.text->width, state.text->height, 0, GL_RGB, GL_UNSIGNED_BYTE, state.text->data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(state.text->data);
    } else {
        glBindTexture(GL_TEXTURE_2D, state.text->def);
        state.text->t1 = state.text->def;
    }

    // Texture 2
    glGenTextures(1, &state.text->t2);
    glBindTexture(GL_TEXTURE_2D, state.text->t2);
    // Texture wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Texture filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image
    state.text->data = stbi_load("res/awesomeface.png", &state.text->width, &state.text->height, &state.text->nrChannels, 0);
    if (state.text->data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, state.text->width, state.text->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, state.text->data);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(state.text->data);
    } else {
        glBindTexture(GL_TEXTURE_2D, state.text->def);
        state.text->t2 = state.text->def;
    }
}

void update_texture()
{
    // Bind textures before rendering
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, state.text->t3);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, state.text->t1);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, state.text->t2);
}

void deinit_texture()
{
    glDeleteTextures(1, &state.text->def);
    if (state.text->t1 != state.text->def) glDeleteTextures(1, &state.text->t1);
    if (state.text->t2 != state.text->def) glDeleteTextures(1, &state.text->t2);
    if (state.text->t3 != state.text->def) glDeleteTextures(1, &state.text->t3);
}
