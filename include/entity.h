#ifndef PORTAL_ENGINE_ENTITY_H
#define PORTAL_ENGINE_ENTITY_H

#include <glm/glm.hpp>

struct Entity {
private:
    glm::mat4 translation_matrix;
    glm::mat4 rotation_matrix;
    glm::mat4 scale_matrix;

    glm::mat4 model_matrix;

    glm::vec3 center;

    void update();

protected:
    Entity();
public:
    void set_center(glm::vec3 center_);

    [[nodiscard]] glm::mat4 get_translation_matrix() const;
    [[nodiscard]] glm::mat4 get_rotation_matrix() const;
    [[nodiscard]] glm::mat4 get_scale_matrix() const;

    [[nodiscard]] glm::mat4 get_model_matrix() const;
    [[nodiscard]] glm::quat get_quaternion_rotation_matrix() const;

    void set_translation_matrix(const glm::mat4 &data);
    void set_rotation_matrix(const glm::mat4 &data);
    void set_scale_matrix(const glm::mat4 &data);
    void set_model_matrix(const glm::mat4 &data);

    void translate(const glm::vec3 &data);
    void rotate(float angle, const glm::vec3 &data);
    void scale(const glm::vec3 &data);

    [[nodiscard]] glm::vec3 get_center() const;

    void copy_data(Entity other);
};

#endif    // PORTAL_ENGINE_ENTITY_H
