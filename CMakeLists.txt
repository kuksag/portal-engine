cmake_minimum_required(VERSION 3.16)
project(portal_engine)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/" ${CMAKE_MODULE_PATH})

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

set(OPENGL_LIBS OpenGL::GL GLFW::GLFW GLEW::GLEW glm::glm SOIL assimp)

add_compile_options(-Wall -Wextra -O2)

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

#add_compile_options(-fsanitize=undefined)
#add_link_options(-fsanitize=undefined)

add_executable(main
        src/main.cpp
        src/camera.cpp include/camera.h
        src/controls.cpp include/controls.h
        include/settings.h
        src/shader.cpp include/shader.h
        src/entity.cpp include/entity.h
        src/drawable.cpp include/drawable.h
        src/mesh.cpp include/mesh.h
        src/Texture.cpp include/Texture.h
        src/model.cpp include/model.h
        src/primitives.cpp include/primitives.h
        src/light_source.cpp include/light_source.h
        src/portal.cpp include/portal.h
        src/puzzle.cpp include/puzzle.h
        src/scene.cpp include/scene.h
        src/window.cpp include/window.h
        src/portal_gun.cpp include/portal_gun.h)

target_include_directories(main PUBLIC include include/primitives src)
target_link_libraries(main ${OPENGL_LIBS})
