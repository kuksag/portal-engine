#include "primitives.h"

#include <glm/glm.hpp>
#include <memory>
#include <string>

Primitive::Primitive(const glm::vec3& position, const glm::vec3& color,
                     const std::string& path, bool need_load,
                     std::shared_ptr<ShaderProgram> shader)
    : Model(path, shader, need_load) {
    translate(position);
    set_color(color);
}

void Primitive::set_color(glm::vec3 color_) {
    color = color_;
    shader->use();
    glUniform4f(shader->get_uniform_id("color"), color.x, color.y, color.z, 1);
}

void Primitive::set_unvisible() { set_color({-1, -1, -1}); }

glm::vec3 Primitive::get_color() const { return color; }

bool Primitive::is_visible() const {
    return color.x >= 0 && color.y >= 0 && color.z >= 0;
}

namespace {
glm::vec2 cross_line_with_xz_plane(glm::vec3 first_point,
                                   glm::vec3 last_point) {
    glm::vec3 directive_vector = last_point - first_point;
    float k = -first_point.y / directive_vector.y;
    return glm::vec2(first_point.x + k * directive_vector.x,
                     first_point.z + k * directive_vector.z);
}
}    // namespace

bool Plane::crossed(glm::vec3 first_point, glm::vec3 last_point) const {
    auto inverse_operator = glm::inverse(get_model_matrix());
    first_point = inverse_operator * glm::vec4(first_point, 1.0f);
    last_point = inverse_operator * glm::vec4(last_point, 1.0f);
    bool crossed_plane = (first_point.y < 0.0 && last_point.y > 0.0) ||
                         (first_point.y > 0.0 && last_point.y < 0.0);
    if (!crossed_plane) {
        return false;
    }
    glm::vec2 intersection = cross_line_with_xz_plane(first_point, last_point);
    return intersection.x >= -1.0 && intersection.x <= 1.0 &&
           intersection.y >= -1.0 && intersection.y <= 1.0;
}

Sphere::Sphere(std::shared_ptr<ShaderProgram> shader, bool need_load,
               const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/sphere.obj", need_load,
                shader) {}
Plane::Plane(std::shared_ptr<ShaderProgram> shader, bool need_load,
             const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/plane.obj", need_load,
                shader) {}

Cylinder::Cylinder(std::shared_ptr<ShaderProgram> shader, bool need_load,
                   const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/cylinder.obj",
                need_load, shader) {}
Torus::Torus(std::shared_ptr<ShaderProgram> shader, bool need_load,
             const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/torus.obj", need_load,
                shader) {}
Cone::Cone(std::shared_ptr<ShaderProgram> shader, bool need_load,
           const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/cone.obj", need_load,
                shader) {}

Cube::Cube(std::shared_ptr<ShaderProgram> shader, bool need_load,
           const glm::vec3& position, const glm::vec3& color)
    : Primitive(position, color, "res/models/primitives/cube.obj", need_load,
                shader) {
    for (std::size_t i = 0; i < 6; ++i) {
        planes.push_back(std::make_shared<Plane>(shader, true));
        planes.back()->link_to(this);
        planes.back()->set_color({0, 0, 0});
    }
    planes[0]->rotate(M_PI, {0, 0, 1});

    planes[1]->translate({0, 2, 0});

    planes[2]->rotate(M_PI_2, {1, 0, 0});
    planes[2]->translate({0, 1, 1});

    planes[3]->rotate(M_PI_2, {-1, 0, 0});
    planes[3]->translate({0, 1, -1});

    planes[4]->rotate(M_PI_2, {0, 0, 1});
    planes[4]->translate({-1, 1, 0});

    planes[5]->rotate(M_PI_2, {0, 0, -1});
    planes[5]->translate({1, 1, 0});
}

const std::vector<std::shared_ptr<Plane>>& Cube::get_planes() const {
    return planes;
}