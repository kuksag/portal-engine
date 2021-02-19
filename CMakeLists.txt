cmake_minimum_required(VERSION 3.17)
project(portal_engine)

set(CMAKE_CXX_STANDARD 17)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/" ${CMAKE_MODULE_PATH})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(SOIL REQUIRED)


find_package(GLFW REQUIRED)
add_library(GLFW INTERFACE)
target_include_directories(GLFW INTERFACE ${GLFW_INCLUDE_DIRS})
target_link_libraries(GLFW INTERFACE ${GLFW_LIBRARIES})
add_library(GLFW::GLFW ALIAS GLFW)

find_library(SOIL_LIBRARY NAMES SOIL)
add_library(SOIL INTERFACE)
target_include_directories(SOIL INTERFACE ${SOIL_INCLUDE_DIRS})
target_link_libraries(SOIL INTERFACE ${SOIL_LIBRARIES})

if (NOT TARGET glm::glm)
    add_library(glm::glm ALIAS glm)
endif ()

set(OPENGL_LIBS OpenGL::GL GLFW::GLFW GLEW::GLEW glm::glm SOIL)


add_executable(main src/main.cpp src/camera.cpp include/camera.h src/controls.cpp include/controls.h include/settings.h src/shader.cpp include/shader.h src/entity.cpp include/entity.h src/drawable.cpp include/drawable.h src/model.cpp include/model.h src/mesh.cpp src/mesh.h)

target_include_directories(main PUBLIC include src)
target_link_libraries(main ${OPENGL_LIBS})
