cmake_minimum_required(VERSION 3.27)
project(so_1)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})

include_directories(glfw/include)

set(OpenGlLinkers  -lGLU -lGL -lglut)
set(CMAKE_CXX_STANDARD 17)

add_executable(so_1 main.cpp
)

include_directories(GL)
target_link_libraries(so_1 GL GLU glut glfw3 X11 Xxf86vm Xrandr pthread Xi dl Xinerama Xcursor assimp)