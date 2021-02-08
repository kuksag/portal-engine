cmake_minimum_required(VERSION 3.17)
project(portal_engine)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
set(OPENGL_LIBS ${OPENGL_LIBRARY} glfw GLEW)

add_executable(main src/main.cpp src/shader-loader.cpp include/shader-loader.h src/camera.cpp include/camera.h)

target_include_directories(main PUBLIC include src)
target_link_libraries(main ${OPENGL_LIBS})
