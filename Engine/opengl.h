#pragma once

#if defined(__APPLE__)
    #include <OpenGL/gl3.h>
    #include <OpenGL/glext.h>
#elif defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <GL/gl.h>
#else
    #define GL_GLEXT_PROTOTYPES 1
    #include <GL/gl.h>
    #include <GL/glext.h>
    #include <GL/glu.h>
#endif
