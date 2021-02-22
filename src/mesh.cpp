#include "mesh.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <vector>

#include "shader.h"
#include <utility>
#include <iostream>

namespace {
template <typename T>
void is_data_provided(const std::vector<T> &data) {
    if (data.empty())
        throw std::logic_error("trying to initialise mesh with empty data");
}
}    // namespace

void Mesh::bind_vertices() {
    is_data_provided(vertices);
    bind();

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);

    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(typeof(vertices.front())),
                 &vertices[0],
                 GL_STATIC_DRAW);

    glVertexAttribPointer(VERTEX_LAYOUT,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(typeof(vertices.front())),
                          nullptr);

    glEnableVertexAttribArray(VERTEX_LAYOUT);

    unbind();
}

void Mesh::bind_indexed_vertices() {
    is_data_provided(indexed_vertices);
    bind();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indexed_vertices.size() * sizeof(typeof(indexed_vertices.front())),
        &indexed_vertices[0],
        GL_STATIC_DRAW);

    unbind();
}

void Mesh::bind_colors() {
    is_data_provided(colors);
    bind();

    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_object);

    glBufferData(GL_ARRAY_BUFFER,
                 colors.size() * sizeof(typeof(colors.front())),
                 &colors[0],
                 GL_STATIC_DRAW);

    glVertexAttribPointer(COLOR_LAYOUT,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(typeof(colors.front())),
                          nullptr);

    glEnableVertexAttribArray(COLOR_LAYOUT);

    unbind();
}

void Mesh::bind_textures() {
    is_data_provided(textures);
    bind();

    glBindBuffer(GL_ARRAY_BUFFER, textures_buffer_object);

    glBufferData(GL_ARRAY_BUFFER,
                 textures.size() * sizeof(typeof(textures.front())),
                 &textures[0],
                 GL_STATIC_DRAW);

    glVertexAttribPointer(TEXTURES_LAYOUT,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          2 * sizeof(typeof(textures.front())),
                          nullptr);

    glEnableVertexAttribArray(TEXTURES_LAYOUT);

    unbind();
}

void Mesh::bind_normals() {
    is_data_provided(normals);
    bind();

    glBindBuffer(GL_ARRAY_BUFFER, normals_buffer_object);

    glBufferData(GL_ARRAY_BUFFER,
                 normals.size() * sizeof(typeof(normals.front())),
                 &normals[0],
                 GL_STATIC_DRAW);

    glVertexAttribPointer(NORMALS_LAYOUT,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          3 * sizeof(typeof(normals.front())),
                          nullptr);

    glEnableVertexAttribArray(NORMALS_LAYOUT);

    unbind();
}


Mesh::Mesh(std::string vertex_shader_name, std::string fragment_shader_name)
    : shader(std::move(vertex_shader_name), std::move(fragment_shader_name)) {
    glGenVertexArrays(1, &vertex_array_object);
    glGenBuffers(1, &vertex_buffer_object);
    glGenBuffers(1, &color_buffer_object);
    glGenBuffers(1, &element_buffer_object);
    glGenBuffers(1, &textures_buffer_object);
    glGenBuffers(1, &normals_buffer_object);
}

void Mesh::bind() const { glBindVertexArray(vertex_array_object); }

void Mesh::unbind() const { glBindVertexArray(0); }

void Mesh::add_vertices(std::vector<float> data) {
    vertices = std::move(data);
    bind_vertices();
}

void Mesh::add_indexed_vertices(std::vector<int> data) {
    indexed_vertices = std::move(data);
    bind_indexed_vertices();
}

void Mesh::add_colors(std::vector<float> data) {
    colors = std::move(data);
    bind_colors();
}

void Mesh::add_textures(std::vector<float> data) {
    textures = std::move(data);
    bind_textures();
}

void Mesh::add_normals(std::vector<float> data) {
    normals = std::move(data);
    bind_normals();
}

int Mesh::get_number_of_vertices() const {
    return vertices.size();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteBuffers(1, &vertex_buffer_object);
    glDeleteBuffers(1, &color_buffer_object);
    glDeleteBuffers(1, &element_buffer_object);
    glDeleteBuffers(1, &textures_buffer_object);
}
