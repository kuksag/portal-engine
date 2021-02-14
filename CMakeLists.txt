cmake_minimum_required(VERSION 3.17)
project(portal_engine)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/" ${CMAKE_MODULE_PATH})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

find_package(GLFW REQUIRED)
add_library(GLFW INTERFACE)
target_include_directories(GLFW INTERFACE ${GLFW_INCLUDE_DIRS})
target_link_libraries(GLFW INTERFACE ${GLFW_LIBRARIES})
add_library(GLFW::GLFW ALIAS GLFW)

if (NOT TARGET glm::glm)
    add_library(glm::glm ALIAS glm)
endif ()

set(OPENGL_LIBS OpenGL::GL GLFW::GLFW GLEW::GLEW glm::glm)

add_executable(main src/main.cpp src/shader-loader.cpp include/shader-loader.h src/camera.cpp include/camera.h src/controls.cpp include/controls.h include/settings.h)

target_include_directories(main PUBLIC include src)
target_link_libraries(main ${OPENGL_LIBS})
