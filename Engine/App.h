#pragma once

// Engine Application API
// Simple interface for creating Vulkan applications

// Main macro
// and the engine handles the rest
void INPUT();
void RENDER();
void RUN();
#define ENGINE_ENTRY_POINT \
    int main() { \
        RUN(); \
        return 0; \
    }

#define TITLE  "opengl _f"
#define WIDTH  1980
#define HEIGHT 1270