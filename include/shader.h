#ifndef PORTAL_ENGINE_SHADER_H
#define PORTAL_ENGINE_SHADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <unordered_map>

struct ShaderProgram {
private:
    GLuint vertex_shader_id;
    GLuint fragment_shader_id;
    GLuint program_id;

    std::unordered_map<std::string, GLuint> uniforms;
    std::unordered_map<std::string, GLuint> attributes;

    // debug info
    std::string vertex_shader_name;
    std::string fragment_shader_name;

public:
    explicit ShaderProgram(const std::string &vertex_shader_name,
                           const std::string &fragment_shader_name);

    void use() const;

    GLuint get_uniform_id(const std::string &uniform_name);
    GLuint get_attribute_id(const std::string &attribute_name);

    GLuint get_id() const;

    ~ShaderProgram();
};

#endif    // PORTAL_ENGINE_SHADER_H
