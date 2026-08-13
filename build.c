#include <cbuild.h>

static void add_imgui(C_Target *app) {
    c_sources(app, "Vendor/imgui/imgui.cpp");
    c_sources(app, "Vendor/imgui/imgui_draw.cpp");
    c_sources(app, "Vendor/imgui/imgui_tables.cpp");
    c_sources(app, "Vendor/imgui/imgui_widgets.cpp");
    c_sources(app, "Vendor/imgui/imgui_impl_glfw.cpp");
    c_sources(app, "Vendor/imgui/imgui_impl_opengl3.cpp");
}

static void add_glfw(C_Target *app) {
    c_sources(app, "Vendor/glfw/src/context.c");
    c_sources(app, "Vendor/glfw/src/init.c");
    c_sources(app, "Vendor/glfw/src/input.c");
    c_sources(app, "Vendor/glfw/src/monitor.c");
    c_sources(app, "Vendor/glfw/src/platform.c");
    c_sources(app, "Vendor/glfw/src/vulkan.c");
    c_sources(app, "Vendor/glfw/src/window.c");
    c_sources(app, "Vendor/glfw/src/egl_context.c");
    c_sources(app, "Vendor/glfw/src/osmesa_context.c");
    c_sources(app, "Vendor/glfw/src/null_init.c");
    c_sources(app, "Vendor/glfw/src/null_monitor.c");
    c_sources(app, "Vendor/glfw/src/null_window.c");
    c_sources(app, "Vendor/glfw/src/null_joystick.c");

#ifdef __APPLE__
    c_define(app, "_GLFW_COCOA");
    c_sources(app, "Vendor/glfw/src/macos_time.c");
    c_sources(app, "Vendor/glfw/src/posix_module.c");
    c_sources(app, "Vendor/glfw/src/posix_thread.c");
    c_sources(app, "Vendor/glfw/src/cocoa_init.m");
    c_sources(app, "Vendor/glfw/src/cocoa_joystick.m");
    c_sources(app, "Vendor/glfw/src/cocoa_monitor.m");
    c_sources(app, "Vendor/glfw/src/cocoa_window.m");
    c_sources(app, "Vendor/glfw/src/nsgl_context.m");
#else
    c_define(app, "_GLFW_X11");
    c_define(app, "_DEFAULT_SOURCE");
    c_sources(app, "Vendor/glfw/src/posix_time.c");
    c_sources(app, "Vendor/glfw/src/posix_module.c");
    c_sources(app, "Vendor/glfw/src/posix_thread.c");
    c_sources(app, "Vendor/glfw/src/x11_init.c");
    c_sources(app, "Vendor/glfw/src/x11_monitor.c");
    c_sources(app, "Vendor/glfw/src/x11_window.c");
    c_sources(app, "Vendor/glfw/src/xkb_unicode.c");
    c_sources(app, "Vendor/glfw/src/glx_context.c");
    c_sources(app, "Vendor/glfw/src/linux_joystick.c");
    c_sources(app, "Vendor/glfw/src/posix_poll.c");
#endif
}

void build(C_Build *b) {
    C_Target *app = c_executable(b, "bge");

    c_sources(app, "main.c");
    c_sources(app, "Engine/App.c");
    c_sources(app, "Engine/cam.c");
    c_sources(app, "Engine/gfx.c");
    c_sources(app, "Engine/text.c");
    c_sources(app, "Engine/level.c");
    c_sources(app, "Engine/portal.c");
    c_sources(app, "Engine/editor.c");
    c_sources(app, "Engine/gun.c");
    c_sources(app, "Engine/render.c");
    c_sources(app, "Engine/util/math.c");
    c_sources(app, "Engine/imgui_c.cpp");
    c_sources(app, "Vendor/glad/src/glad.c");
    add_imgui(app);
    add_glfw(app);

    c_include(app, ".");
    c_include(app, "Engine");
    c_include(app, "Vendor/glad/include");
    c_include(app, "Vendor/glfw/include");
    c_include(app, "Vendor/glfw/src");
    c_include(app, "Vendor/imgui");

    c_define(app, "GL_SILENCE_DEPRECATION");
    c_flag(app, "-std=c++11");
    c_flag(app, "-Wall");
    c_flag(app, "-Wextra");

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
