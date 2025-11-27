cmake_minimum_required(VERSION 4.0)
project(GLplay)

set(CMAKE_CXX_STANDARD 26)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_library(glad-core glad-core/src/glad.c)
add_library(glad-compat glad-compat/src/glad.c)

add_executable(GLplay-core core.cpp
        constants.hpp)
target_link_libraries(GLplay-core glad-core glfw)

add_executable(GLplay-compat compat.cpp
        constants.hpp)
target_link_libraries(GLplay-compat glad-compat glfw)

add_executable(GLplay-glut glut-compat.cpp)
target_link_libraries(GLplay-glut GLUT::GLUT OpenGL::GLU)
