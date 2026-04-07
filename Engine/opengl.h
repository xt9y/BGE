#pragma once

#if defined(__APPLE__)
    #include <OpenGL/gl3.h>
    #include <OpenGL/glext.h>
#elif defined(_WIN32)
    #define GLFW_INCLUDE_NONE
    #define GLFW_INCLUDE_NO_GLU
    #include <GLFW/glfw3.h>
#else
    #define GL_GLEXT_PROTOTYPES 1
    #include <GL/gl.h>
    #include <GL/glext.h>
    #include <GL/glu.h>
#endif
