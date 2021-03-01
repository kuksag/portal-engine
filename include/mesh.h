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
    static const int NORMALS_LAYOUT = 3;

    GLuint vertex_array_object{};

    GLuint vertex_buffer_object{};
    GLuint color_buffer_object{};
    GLuint element_buffer_object{};
    GLuint textures_buffer_object{};
    GLuint normals_buffer_object{};

    std::vector<float> vertices;
    std::vector<float> colors;
    std::vector<int> indexed_vertices;
    std::vector<float> textures;
    std::vector<float> normals;

    void bind_vertices();
    void bind_indexed_vertices();
    void bind_colors();
    void bind_textures();
    void bind_normals();

public:
    Mesh();

    void bind() const;
    void unbind() const;

    void add_vertices(std::vector<float> data);
    void add_indexed_vertices(std::vector<int> data);
    void add_colors(std::vector<float> colors);
    void add_textures(std::vector<float> data);
    void add_normals(std::vector<float> normals);

    int get_number_of_vertices() const;

    ~Mesh();

    // TODO: load_model();
};

#endif    // PORTAL_ENGINE_MESH_H
