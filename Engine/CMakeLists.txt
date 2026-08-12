cmake_minimum_required(VERSION 3.10)
project(Engine C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)
if(NOT WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -W")
endif()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
add_subdirectory(../Vendor/glfw ${CMAKE_BINARY_DIR}/glfw)
add_subdirectory(../Vendor/imgui ${CMAKE_BINARY_DIR}/imgui)

add_library(engine STATIC
    App.c cam.c gfx.c text.c level.c portal.c editor.c gun.c render.c util/math.c
    imgui_c.cpp
)

target_sources(engine PRIVATE ../Vendor/glad/src/glad.c)
target_include_directories(engine PUBLIC ../Vendor/glad/include)

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ..)
target_link_libraries(engine PUBLIC glfw imgui)

include(GNUInstallDirs)
install(TARGETS engine
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
