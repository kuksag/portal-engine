#include "primitives.h"

#include <glm/glm.hpp>
#include <memory>
#include <string>

Primitive::Primitive(const glm::vec3& position, const glm::vec3& color,
                     const std::string& path, bool need_load, std::shared_ptr<ShaderProgram> shader)
    : Model(path, shader, need_load) {
    translate(position);
    set_color(color);
}

void Primitive::set_color(glm::vec3 color_) {
    color = color_;
    shader->use();
    glUniform4f(shader->get_uniform_id("color"), color.x, color.y, color.z, 1);
}

void Primitive::set_unvisible() {
    set_color({-1, -1, -1});
}

glm::vec3 Primitive::get_color() const { return color; }

Sphere::Sphere(std::shared_ptr<ShaderProgram> shader, bool need_load, const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/sphere.obj", need_load, shader) {}
Cube::Cube(std::shared_ptr<ShaderProgram> shader, bool need_load, const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/cube.obj", need_load, shader) {}
Plane::Plane(std::shared_ptr<ShaderProgram> shader, bool need_load, const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/plane.obj", need_load, shader) {}
Cylinder::Cylinder(std::shared_ptr<ShaderProgram> shader, bool need_load, const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/cylinder.obj", need_load, shader) {}
Torus::Torus(std::shared_ptr<ShaderProgram> shader, bool need_load, const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/torus.obj", need_load, shader) {}
Cone::Cone(std::shared_ptr<ShaderProgram> shader, bool need_load, const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/cone.obj", need_load, shader) {}