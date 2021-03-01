#include "entity.h"

#include <glm/gtc/matrix_transform.hpp>

Entity::Entity()
    : translation_matrix(glm::mat4(1.0f)),
      rotation_matrix(glm::mat4(1.0f)),
      scale_matrix(glm::mat4(1.0f)),
      model_matrix(glm::mat4(1.0f)) {}

void Entity::update() {
    model_matrix = translation_matrix * rotation_matrix * scale_matrix;
}

glm::mat4 Entity::get_translation_matrix() const { return translation_matrix; }

glm::mat4 Entity::get_model_matrix() const { return model_matrix; }

void Entity::set_translation_matrix(const glm::mat4 &data) {
    translation_matrix = data;
    update();
}

void Entity::set_rotation_matrix(const glm::mat4 &data) {
    rotation_matrix = data;
    update();
}

void Entity::set_scale_matrix(const glm::mat4 &data) { scale_matrix = data; }

void Entity::translate(const glm::vec3 &data) {
    translation_matrix = glm::translate(translation_matrix, data);
    update();
}

void Entity::rotate(float angle, const glm::vec3 &data) {
    rotation_matrix = glm::rotate(rotation_matrix, angle, data);
    update();
}

void Entity::scale(const glm::vec3 &data) {
    scale_matrix = glm::scale(scale_matrix, data);
    update();
}

glm::vec3 Entity::get_center() const {
    auto result = get_model_matrix() * glm::vec4(center, 1.0f);
    return glm::vec3(result[0], result[1], result[2]);
}

void Entity::set_center(glm::vec3 center_) {
    center = center_;
}
