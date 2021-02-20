#include "entity.h"

Entity::Entity()
    : translation_matrix(glm::mat4(1.0f)),
      rotation_matrix(glm::mat4(1.0f)),
      scale_matrix(glm::mat4(1.0f)),
      model_matrix(translation_matrix * rotation_matrix * scale_matrix) {}

glm::mat4 Entity::get_model_matrix() const {
    return model_matrix;
}
