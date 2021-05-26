#include "portal.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

#include "scene.h"
#include "settings.h"

Portal::Portal(Scene *scene, std::shared_ptr<ShaderProgram> shader)
    : Drawable(shader), destination(this) {
    // -------------------------------------------------------------------------
    shape = scene->add_primitive<Plane>();
    shape->rotate(M_PI_2, {1.0, 0.0, 0.0});
    shape->rotate(M_PI, {0.0, 0.0, 1.0});
    shape->link_to(this);
    shape->set_unvisible();
    // -------------------------------------------------------------------------
    // beacon.scale({0.2, 0.2, 0.2});
    // beacon.set_color({0.0, 1.0, 0.0});
    // beacon.rotate(M_PI_2, {0.0, 1.0, 0.0});
    // beacon.rotate(-M_PI_2, {0.0, 0.0, 1.0});
    // beacon.translate({0.0, 0.0, -0.5});
    // -------------------------------------------------------------------------
    const float COLOR = 0.75;    // grey
    const float X_SCALE = 1.21;
    const float YZ_SCALE = 0.1;
    const float OFFSET = 1.11;
    const std::size_t EDGE_NUMBER = 4;

    bounds.resize(EDGE_NUMBER);
    for (auto &bound : bounds) {
        bound = scene->add_primitive<Cube>();
        bound->scale({X_SCALE, YZ_SCALE, YZ_SCALE});
        bound->set_color({glm::vec3(COLOR)});
        bound->link_to(this);
    }

    bounds[0]->translate({0.0, -OFFSET, 0.0});
    bounds[1]->translate({0.0, OFFSET, 0.0});

    bounds[2]->translate({-OFFSET, 0.0, 0.0});
    bounds[2]->rotate(M_PI_2, {0.0, 0.0, 1.0});

    bounds[3]->translate({OFFSET, 0.0, 0.0});
    bounds[3]->rotate(M_PI_2, {0.0, 0.0, 1.0});
    // -------------------------------------------------------------------------
}

void Portal::draw(const Camera &camera,
                  const std::vector<std::shared_ptr<LightSource> > &ls) const {}

void Portal::draw1(const Camera &camera) const { shape->draw(camera, {}); }

void Portal::set_destination(Portal *other) { destination = other; }

glm::vec3 Portal::get_center() const {
    auto result = get_model_matrix() * glm::vec4(glm::vec3(0.0f), 1.0f);
    return glm::vec3(result[0], result[1], result[2]);
}

Camera get_portal_destination_camera(const Camera &camera,
                                     const Portal &portal) {
    auto result = camera;
    result.set_view_matrix(
        // 3. Transformation from source portal to the camera -- it's the
        //    first portal's ModelView matrix:
        camera.get_view_matrix() * portal.get_model_matrix() *
        // 2. Object is front-facing, the camera is facing the other way:
        glm::rotate(glm::mat4(1.0), static_cast<float>(M_PI),
                    glm::quat_cast(portal.get_rotation_matrix()) *
                        glm::vec3(0.0, 1.0, 0.0)) *
        // 1. Go the destination portal; using inverse, because camera
        //    transformations are reversed compared to object
        //    transformations:
        glm::inverse(portal.destination->get_model_matrix()));
    return result;
}

void Portal::depth_test_draw(
    const Camera &camera, std::shared_ptr<ShaderProgram> depth_shader) const {}

namespace {
glm::vec2 cross_line_with_zero_plane(glm::vec3 first_point,
                                     glm::vec3 last_point) {
    glm::vec3 directive_vector = last_point - first_point;
    float k = -first_point.z / directive_vector.z;
    return glm::vec2(first_point.x + k * directive_vector.x,
                     first_point.y + k * directive_vector.y);
}
}    // namespace

bool Portal::crossed(glm::vec3 first_point, glm::vec3 last_point) const {
    auto inverse_operator = glm::inverse(get_model_matrix());
    first_point = inverse_operator * glm::vec4(first_point, 1.0f);
    last_point = inverse_operator * glm::vec4(last_point, 1.0f);
    // using std::swap;
    // swap(first_point.y, first_point.z);
    // swap(last_point.y, last_point.z);
    bool crossed_plane = (first_point.z < 0.0 && last_point.z > 0.0) ||
                         (first_point.z > 0.0 && last_point.z < 0.0);
    if (!crossed_plane) {
        return false;
    }
    glm::vec2 intersection =
        cross_line_with_zero_plane(first_point, last_point);
    return intersection.x >= -1.0 && intersection.x <= 1.0 &&
           intersection.y >= -1.0 && intersection.y <= 1.0;
}
Portal *const Portal::get_destination() { return destination; }
glm::vec3 Portal::get_normal() const {
    return get_rotation_matrix() * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
}
