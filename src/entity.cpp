#include "entity.h"

Entity::Entity()
    : translation_matrix(glm::mat4(0.0f)),
      rotation_matrix(glm::mat4(0.0f)),
      scale_matrix(glm::mat4(1.0f)) {}
