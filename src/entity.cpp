#include "entity.h"
#include <cassert>

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

glm::mat4 Entity::get_rotation_matrix() const { return rotation_matrix; }

glm::mat4 Entity::get_scale_matrix() const { return scale_matrix; }

void Entity::set_model_matrix(const glm::mat4 &data) {
    model_matrix = data;
    // alert! next update will undo this action
}
void Entity::move(float time_delta) {
}
glm::vec3 Entity::get_position() const {
    return get_model_matrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}
