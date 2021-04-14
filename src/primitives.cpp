#include "primitives.h"

#include <glm/glm.hpp>
#include <memory>
#include <string>

Primitive::Primitive(const glm::vec3& position, const glm::vec3& color,
                     const std::string& path, bool need_load)
    : Model(path, std::make_shared<ShaderProgram>("shaders/light.vertex",
                                                  "shaders/light.fragment"), need_load) {
    translate(position);
    set_color(color);
}

void Primitive::move_to(std::shared_ptr<Primitive> to)
{
    set_color(to->color);
    set_model_matrix(to->get_model_matrix());
}

void Primitive::set_color(glm::vec3 color_) {
    color = color_;
    shader->use();
    glUniform4f(shader->get_uniform_id("color"), color.x, color.y, color.z, 1);
}

glm::vec3 Primitive::get_color() const { return color; }

Sphere::Sphere(bool need_load, const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/sphere.obj", need_load) {}
Cube::Cube(bool need_load, const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/cube.obj", need_load) {}
Plane::Plane(bool need_load, const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/plane.obj", need_load) {}
Cylinder::Cylinder(bool need_load, const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/cylinder.obj", need_load) {}
Torus::Torus(bool need_load, const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/torus.obj", need_load) {}
Cone::Cone(bool need_load, const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/cone.obj", need_load) {}