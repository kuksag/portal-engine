#ifndef PORTAL_ENGINE_SHADER_LOADER_H
#define PORTAL_ENGINE_SHADER_LOADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>

GLuint load_shader(const std::string &vertex_shader_file_path,
                   const std::string &fragment_shader_file_path);

#endif    // PORTAL_ENGINE_SHADER_LOADER_H
