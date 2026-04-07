cmake_minimum_required(VERSION 3.10)
project(Engine C)

set(CMAKE_C_STANDARD 99)
if(NOT WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -W")
endif()

add_library(engine STATIC
    App.c cam.c gfx.c text.c level.c editor.c util/math.c
)

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ..)
target_link_libraries(engine PUBLIC glfw)
