#include "primitives.h"

#include <glm/glm.hpp>
#include <memory>
#include <string>

Primitive::Primitive(const glm::vec3& position, const glm::vec3& color,
                     const std::string& path)
    : Model(path, std::make_shared<ShaderProgram>("shaders/light.vertex",
                                                  "shaders/light.fragment")) {
    translate(position);
    set_color(color);
}

void Primitive::set_color(glm::vec3 color_) {
    color = color_;
    shader->use();
    glUniform4f(shader->get_uniform_id("color"), color.x, color.y, color.z, 1);
}

glm::vec3 Primitive::get_color() const { return color; }

Sphere::Sphere(const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/sphere.obj") {}
Cube::Cube(const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/cube.obj") {}
Plane::Plane(const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/plane.obj") {}
Cylinder::Cylinder(const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/cylinder.obj") {}
Torus::Torus(const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/torus.obj") {}
Cone::Cone(const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/cone.obj") {}
