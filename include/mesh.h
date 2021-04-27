#ifndef PORTAL_ENGINE_MESH_H
#define PORTAL_ENGINE_MESH_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    GLuint id;
};

class Mesh
{
private:

    std::vector<Vertex> vertexes;
    std::vector<GLuint> indexes;
    std::vector<Texture> textures;
    
    GLuint VAO, VBO, EBO;

    std::shared_ptr<ShaderProgram> shader;

public:

    Mesh(const std::vector<Vertex>& vertexes, const std::vector<GLuint>& indexes, const std::vector<Texture>& textures, std::shared_ptr<ShaderProgram> shader);

    void draw() const;
    void depth_test_draw(std::shared_ptr<ShaderProgram> depth_shader) const;
    
};

#endif    // PORTAL_ENGINE_MESH_H
