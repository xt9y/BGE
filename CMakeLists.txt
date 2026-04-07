cmake_minimum_required(VERSION 3.10)
project(openglc C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -w -DGL_SILENCE_DEPRECATION")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_X11 ON CACHE BOOL "" FORCE)
add_subdirectory(Vendor/glfw)

add_subdirectory(Engine)

add_executable(openglc main.c)
target_link_libraries(openglc PRIVATE engine)

if(UNIX)
    target_link_libraries(openglc PRIVATE m)
endif()

if(WIN32)
    target_include_directories(openglc PRIVATE ${CMAKE_SOURCE_DIR}/Vendor/glad/include)
endif()

add_custom_command(TARGET openglc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json
    COMMENT "Copying compile_commands.json to project root"
)

if(APPLE)
    find_library(OPENGL_FRAMEWORK OpenGL REQUIRED)
    target_link_libraries(openglc PRIVATE ${OPENGL_FRAMEWORK})
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(openglc PRIVATE OpenGL::GL dl pthread)
endif()
