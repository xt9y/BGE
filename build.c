#include <cbuild.h>

void build(C_Build *b) {
    C_Target *app = c_executable(b, "bge");
    c_sources(app, "main.c");
    c_include(app, ".");
    c_include(app, "Engine");
    c_include(app, "Vendor/glad/include");
    c_include(app, "Vendor/glfw/include");
    c_include(app, "Vendor/imgui");
    c_define(app, "GL_SILENCE_DEPRECATION");

    C_Dependency *engine = c_git(
        b, "bge-engine", "https://github.com/xt9y/BGE.git",
        "opengl-example-game-engine"
    );
    c_dep_cmake(engine);
    c_dep_subdir(engine, "Engine");
    c_dep_cmake_option(engine, "-DGLFW_INSTALL=ON");
    c_dep_link(engine, "engine");
    c_dep_link(engine, "imgui");
    c_dep_link(engine, "glfw3");
    c_use(app, engine);

#ifdef __APPLE__
    c_link_system(app, "c++");
    c_framework(app, "OpenGL");
    c_framework(app, "Cocoa");
    c_framework(app, "IOKit");
    c_framework(app, "CoreFoundation");
    c_framework(app, "CoreVideo");
    c_framework(app, "QuartzCore");
#else
    c_link_system(app, "stdc++");
    c_link_system(app, "GL");
    c_link_system(app, "m");
    c_link_system(app, "pthread");
    c_link_system(app, "dl");
    c_link_system(app, "rt");
    c_link_system(app, "X11");
    c_link_system(app, "Xrandr");
    c_link_system(app, "Xi");
    c_link_system(app, "Xinerama");
    c_link_system(app, "Xcursor");
    c_link_system(app, "Xext");
#endif
}
