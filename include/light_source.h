#ifndef PORTAL_ENGINE_LIGHT_SOURCE_H
#define PORTAL_ENGINE_LIGHT_SOURCE_H

#include <glm/gtc/matrix_transform.hpp>
#include "entity.h"
#include "camera.h"
#include <memory>
#include "shader.h"
#include "entity.h"
#include <vector>

struct LightSource : public Entity {
private:
    glm::vec3 pos;
    glm::vec3 color;
    std::shared_ptr<ShaderProgram> shader;

    GLuint VBO, VAO;

    std::vector<GLfloat> vertexes{
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  0.5f,  0.0f
    };

public:
    LightSource(glm::vec3 pos, glm::vec3 color, std::shared_ptr<ShaderProgram> shader);
    glm::vec3 get_pos() const;
    glm::vec3 get_color() const;
    void draw(const Camera &camera);
};

#endif    // PORTAL_ENGINE_LIGHT_SOURCE_H
