cmake_minimum_required(VERSION 3.17)
project(portal_engine)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/" ${CMAKE_MODULE_PATH})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(SOIL REQUIRED)
find_package(GLFW REQUIRED)
find_package(ASSIMP REQUIRED)

add_library(GLFW INTERFACE)
target_include_directories(GLFW INTERFACE ${GLFW_INCLUDE_DIRS})
target_link_libraries(GLFW INTERFACE ${GLFW_LIBRARIES})
add_library(GLFW::GLFW ALIAS GLFW)

add_library(SOIL INTERFACE)
target_include_directories(SOIL INTERFACE ${SOIL_INCLUDE_DIRS})
target_link_libraries(SOIL INTERFACE ${SOIL_LIBRARIES})

add_library(ASSIMP INTERFACE)
target_include_directories(ASSIMP INTERFACE ${ASSIMP_INCLUDE_DIRS})
target_link_libraries(ASSIMP INTERFACE ${ASSIMP_LIBRARIES})
add_library(ASSIMP::ASSIMP ALIAS ASSIMP)

if (NOT TARGET glm::glm)
    add_library(glm::glm ALIAS glm)
endif ()

set(OPENGL_LIBS OpenGL::GL GLFW::GLFW GLEW::GLEW glm::glm SOIL assimp)

add_executable(main src/main.cpp src/shader-loader.cpp src/camera.cpp src/controls.cpp)

target_include_directories(main PUBLIC include src)
target_link_libraries(main ${OPENGL_LIBS})
