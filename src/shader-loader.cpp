#include "include/shader-loader.h"

#include <fstream>
#include <iostream>
#include <sstream>
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
        std::cerr << "Impossible to open: " << file_path << std::endl;
        return "";
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
        std::cerr << &shader_error[0] << std::endl;
    }
}

}    // namespace

GLuint load_shader(const std::string &vertex_shader_file_path,
                   const std::string &fragment_shader_file_path) {
    GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

    compile_shader(vertex_shader_id, vertex_shader_file_path);
    compile_shader(fragment_shader_id, fragment_shader_file_path);

    // link the program
    std::cout << "Linking program" << std::endl;
    GLuint program_id = glCreateProgram();
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
        std::cerr << &program_error[0] << std::endl;
    }

    glDetachShader(program_id, vertex_shader_id);
    glDetachShader(program_id, fragment_shader_id);

    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program_id;
}