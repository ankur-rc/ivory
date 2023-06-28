cmake_minimum_required(VERSION 3.10)

project(ivory)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(GLEW REQUIRED)
find_package(glfw3 3 REQUIRED)
find_package(glm REQUIRED)

add_library(ivory SHARED src/window.cpp
                         src/window_manager.cpp
                         src/shader_utils.cpp)

target_include_directories(ivory PUBLIC include)
target_link_libraries(ivory PUBLIC GL GLEW glfw)
target_compile_options(ivory PRIVATE -DSHADER_ROOT="${CMAKE_CURRENT_SOURCE_DIR}/shaders")

add_subdirectory(apps)