#ifndef PORTAL_ENGINE_MESH_H
#define PORTAL_ENGINE_MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <vector>

#include "shader.h"

struct Mesh {
private:
    static const int VERTEX_LAYOUT = 0;
    static const int COLOR_LAYOUT = 1;
    static const int TEXTURES_LAYOUT = 2;

    GLuint vertex_array_object{};
    GLuint vertex_buffer_object{};
    GLuint color_buffer_object{};
    GLuint element_buffer_object{};
    GLuint textures_buffer_object{};

    std::vector<float> vertices;
    std::vector<int> indexed_vertices;
    std::vector <float> textures;
    // TODO: should be vec4 <-> RGBA
    std::vector<float> colors;

    // TODO: add struct texture; handle more attributes
    //    std::vector<glm::vec2> UV;
    //    std::vector<glm::vec3> normals;

    void bind_vertices();
    void bind_indexed_vertices();
    void bind_colors();
    void bind_textures();

public:
    ShaderProgram shader;

    Mesh(std::string vertex_shader_name,
         std::string fragment_shader_name);

    void bind() const;
    void unbind() const;

    void add_vertices(std::vector<float> data);
    void add_indexed_vertices(std::vector<int> data);
    void add_colors(std::vector<float> colors);
    void add_textures(std::vector <float> data);

    int get_number_of_vertices() const;

    ~Mesh();

    // TODO: load_model();
};

#endif    // PORTAL_ENGINE_MESH_H
