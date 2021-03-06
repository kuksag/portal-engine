#ifndef PORTAL_ENGINE_LIGHT_SOURCE_H
#define PORTAL_ENGINE_LIGHT_SOURCE_H

#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "entity.h"

struct LightSource : public Entity {
private:
    glm::vec3 color;

public:
    LightSource(glm::vec3 position_, glm::vec3 color_);
    [[nodiscard]] glm::vec3 get_position() const;
    [[nodiscard]] glm::vec3 get_color() const;
};

#endif    // PORTAL_ENGINE_LIGHT_SOURCE_H
