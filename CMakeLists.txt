cmake_minimum_required(VERSION 3.17)
project(portal_engine)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

add_subdirectory(libs)

include_directories(
        libs/AntTweakBar-1.16/include/
        libs/glfw-3.1.2/include/
        libs/glm-0.9.7.1/
        libs/glew-1.13.0/include/
        libs/assimp-3.0.1270/include/
        .
)

set(ALL_LIBS ${OPENGL_LIBRARY} glfw GLEW_1130)


include_directories(common)

add_executable(run_main src/main.cpp)
target_link_libraries(run_main ${ALL_LIBS})
