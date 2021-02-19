#ifndef PORTAL_ENGINE_ENTITY_H
#define PORTAL_ENGINE_ENTITY_H

#include <glm/glm.hpp>

struct Entity {
private:
    glm::mat4 translation_matrix;
    glm::mat4 rotation_matrix;
    glm::mat4 scale_matrix;

public:
    Entity();
};

#endif    // PORTAL_ENGINE_ENTITY_H
