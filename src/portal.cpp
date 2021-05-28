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

void Portal::draw([[maybe_unused]] const Camera& camera, [[maybe_unused]] const std::vector< std::shared_ptr<LightSource> >& ls) const {
    assert(false);
    // because drawable is an abstract class
}

void Portal::draw1(const Camera &camera) const {
    shape->draw(camera, {});
}

void Portal::set_destination(Portal *other) { destination = other; }

glm::vec3 Portal::get_center() const {
    auto result = get_model_matrix() * glm::vec4(glm::vec3(0.0f), 1.0f);
    return glm::vec3(result[0], result[1], result[2]);
}

bool Portal::is_support() const { return support; }
void Portal::set_is_support(bool support) { this->support = support; }

Camera get_portal_destination_camera(const Camera &camera,
                                     const Portal &portal) {
    auto result = camera;
    result.set_view_matrix(
        // 3. Transformation from source portal to the camera -- it's the
        //    first portal's ModelView matrix:
        camera.get_view_matrix() * portal.get_model_matrix() *
        // 2. Object is front-facing, the camera is facing the other way:
        glm::rotate(glm::mat4(1.0), static_cast<float>(M_PI),
                        glm::vec3(0.0, 1.0, 0.0)) *
        // 1. Go the destination portal; using inverse, because camera
        //    transformations are reversed compared to object
        //    transformations:
        glm::inverse(portal.destination->get_model_matrix()));
    return result;
}

void Portal::depth_test_draw(
    [[maybe_unused]] const Camera &camera, [[maybe_unused]] std::shared_ptr<ShaderProgram> depth_shader) const {
    assert(false);
    // because drawable is an abstract class
}

bool Portal::crossed(glm::vec3 first_point, glm::vec3 last_point) const {
    return dynamic_cast<Plane*>(shape.get())->crossed(first_point, last_point);
}
const Portal *Portal::get_destination() const {
    return destination;
}
glm::vec3 Portal::get_normal() const {
    return get_rotation_matrix() * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
}
