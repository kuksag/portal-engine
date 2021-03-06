#ifndef PORTAL_ENGINE_ENTITY_H
#define PORTAL_ENGINE_ENTITY_H

#include <glm/glm.hpp>
#include <memory>

struct Entity {
private:
    glm::mat4 translation_matrix;
    glm::mat4 rotation_matrix;
    glm::mat4 scale_matrix;

    glm::mat4 model_matrix;

    void update();

protected:
    Entity *link = nullptr;

public:
    Entity();
    [[nodiscard]] glm::mat4 get_translation_matrix() const;
    [[nodiscard]] glm::mat4 get_rotation_matrix() const;
    [[nodiscard]] glm::mat4 get_scale_matrix() const;
    [[nodiscard]] glm::mat4 get_model_matrix() const;

    void set_translation_matrix(const glm::mat4 &data);
    void set_rotation_matrix(const glm::mat4 &data);
    void set_scale_matrix(const glm::mat4 &data);
    void set_model_matrix(const glm::mat4 &data);

    void link_to(Entity *link);

    virtual void translate(const glm::vec3 &data);
    virtual void rotate(float angle, const glm::vec3 &data);
    virtual void scale(const glm::vec3 &data);
    virtual void move(float time_delta);
    glm::vec3 get_position() const;

    virtual ~Entity() = default;
};

#endif    // PORTAL_ENGINE_ENTITY_H
