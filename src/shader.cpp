#include "shader.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include <vector>

namespace {
std::string read_file(const std::string &file_path) {
    std::ifstream file_stream(file_path, std::ios::in);
    if (file_stream.is_open()) {
        std::stringstream sstr;
        sstr << file_stream.rdbuf();
        file_stream.close();
        return sstr.str();
    } else {
        throw std::logic_error("Impossible to open: " + file_path);
    }
}

void compile_shader(GLuint &shader_id, const std::string &shader_file_path) {
    // read shader data
    auto shader_code = read_file(shader_file_path);

    // compile vertex shader
    std::cout << "Compiling shader: " << shader_file_path << std::endl;
    char const *vertex_shader_source = shader_code.c_str();
    glShaderSource(shader_id, 1, &vertex_shader_source, nullptr);
    glCompileShader(shader_id);

    // check if compilation succeed
    GLint compilation_success = GL_FALSE;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compilation_success);
    if (compilation_success == GL_FALSE) {
        int info_log_length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> shader_error(info_log_length + 1);
        glGetShaderInfoLog(shader_id, info_log_length, nullptr,
                           &shader_error[0]);
        throw std::logic_error(&shader_error[0]);
    }
}

std::string logic_error_message(const std::string &shader_name,
                                const std::string &parameter) {
    return "no such parameter in " + shader_name + " such as " + parameter;
}

}    // namespace

ShaderProgram::ShaderProgram(std::string vertex_shader_name_,
                             std::string fragment_shader_name_) {
    const char BASE_PATH[] = "res/";

    vertex_shader_name = std::move(vertex_shader_name_);
    fragment_shader_name = std::move(fragment_shader_name_);

    vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    compile_shader(vertex_shader_id, BASE_PATH + vertex_shader_name);
    compile_shader(fragment_shader_id, BASE_PATH + fragment_shader_name);

    // link the program
    program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader_id);
    glAttachShader(program_id, fragment_shader_id);
    glLinkProgram(program_id);

    // check the program
    GLint link_success = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &link_success);
    if (link_success == GL_FALSE) {
        int info_log_length;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
        std::vector<char> program_error(info_log_length + 1);
        glGetProgramInfoLog(program_id, info_log_length, nullptr,
                            &program_error[0]);
        throw std::logic_error(&program_error[0]);
    }
}

void ShaderProgram::use() const {
    if (vertex_shader_name.empty())
        throw std::logic_error("shader was not initialised");
    glUseProgram(program_id);
}

GLuint ShaderProgram::get_uniform_id(const std::string &uniform_name) {
    if (uniforms.find(uniform_name) != uniforms.end()) {
        return uniforms[uniform_name];
    } else {
        GLuint uniform_id =
            glGetUniformLocation(program_id, uniform_name.c_str());
        if (uniform_id == -1) {
            throw std::logic_error(
                logic_error_message(vertex_shader_name, uniform_name));
        }
        return uniforms[uniform_name] = uniform_id;
    }
}

GLuint ShaderProgram::get_attribute_id(const std::string &attribute_name) {
    if (attributes.find(attribute_name) != attributes.end()) {
        return attributes[attribute_name];
    } else {
        GLuint attribute_id =
            glGetAttribLocation(program_id, attribute_name.c_str());
        if (attribute_id == -1) {
            throw std::logic_error(
                logic_error_message(vertex_shader_name, attribute_name));
        }
        return attributes[attribute_name] = attribute_id;
    }
}

GLuint ShaderProgram::get_id() const { return program_id; }

ShaderProgram::~ShaderProgram() {
    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);
    glDeleteProgram(program_id);
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);
}