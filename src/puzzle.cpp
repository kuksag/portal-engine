#include "puzzle.h"

#include "portal.h"

JokersTrap::JokersTrap(Scene& scene, const glm::vec3& position,
                       std::shared_ptr<ShaderProgram> shader)
    : Drawable(shader), base(scene) {
    const float SCALE_OBJECT = 0.8;

    if (patterns.empty()) {
        for (std::size_t i = 0; i < EDGE_NUMBER; i++) {
            patterns.emplace_back(scene);
        }

        patterns[0].centroid =
            scene.add_primitive<Sphere>(glm::vec3(0.0), {0.44, 0.12, 0.78});
        patterns[1].centroid =
            scene.add_primitive<Torus>(glm::vec3(0.0), {0.16, 0.95, 0.04});
        patterns[2].centroid =
            scene.add_primitive<Cone>(glm::vec3(0.0), {0.88, 0.57, 0.52});
        patterns[3].centroid =
            scene.add_primitive<Cylinder>(glm::vec3(0.0), {0.2, 0.03, 0.92});
        patterns[4].centroid =
            scene.add_primitive<Cone>(glm::vec3(0.0), {0, 0.8, 0.16});
        patterns[5].centroid =
            scene.add_primitive<Cube>(glm::vec3(0.0), {0.2, 0.66, 0.66});

        patterns[3].centroid->scale(glm::vec3(0.6));
        patterns[5].centroid->scale(glm::vec3(0.6));

        for (std::size_t i = 0; i < EDGE_NUMBER; i++) {
            patterns[i].centroid->link_to(&patterns[i]);
            for (std::size_t j = 0; j < EDGE_NUMBER; j++)
                patterns[i].portals[j]->link_to(&patterns[i]);
            patterns[i].centroid->scale(glm::vec3(SCALE_OBJECT));
            patterns[i].translate(
                glm::vec3(static_cast<float>(i + 1) * MOVE_DISTANCE));
            patterns[i].inverse_rotate();
            base.portals[i]->set_destination(patterns[i].portals[i].get());
        }
    }
    // -------------------------------------------------------------------------
    for (std::size_t i = 0; i < EDGE_NUMBER; i++) {
        base.portals[i]->set_destination(patterns[i].portals[i].get());
        base.portals[i]->link_to(&base);
        base.portals[i]->set_is_support(false);
    }

    base.portals[4]->set_destination(patterns[4].portals[4].get());
    base.portals[5]->set_destination(patterns[5].portals[5].get());
    base.link_to(this);
    // -------------------------------------------------------------------------
    translate(position);
}

JokersTrap::PortalsCube::PortalsCube(Scene& scene) {
    const float BOUND_MATCH = 0.9;

    static const float x_offset[] = {0, 0, 1, -1, 0, 0};
    static const float y_offset[] = {0, 0, 0, 0, 1, -1};
    static const float z_offset[] = {1, -1, 0, 0, 0, 0};

    portals.resize(EDGE_NUMBER);
    for (auto& i : portals) {
        i = scene.add_portal();
        i->set_is_support(true);
    }

    for (std::size_t j = 0; j < EDGE_NUMBER; j++) {
        portals[j]->translate({x_offset[j], y_offset[j], z_offset[j]});
        portals[j]->scale({glm::vec3(BOUND_MATCH)});
        // This scale is needed in order to have a perfect cube
    }

    portals[0]->rotate(M_PI, {0.0, 1.0, 0.0});
    portals[2]->rotate(-M_PI_2, {0.0, 1.0, 0.0});
    portals[3]->rotate(M_PI_2, {0.0, 1.0, 0.0});
    portals[4]->rotate(M_PI_2, {1.0, 0.0, 0.0});
    portals[5]->rotate(-M_PI_2, {1.0, 0.0, 0.0});
}

void JokersTrap::PortalsCube::inverse_rotate() {
    portals[0]->rotate(M_PI, {0.0, 1.0, 0.0});
    portals[1]->rotate(M_PI, {0.0, 1.0, 0.0});
    portals[2]->rotate(M_PI, {0.0, 1.0, 0.0});
    portals[3]->rotate(M_PI, {0.0, 1.0, 0.0});
    portals[4]->rotate(M_PI, {1.0, 0.0, 0.0});
    portals[5]->rotate(M_PI, {1.0, 0.0, 0.0});
}

void JokersTrap::draw(const Camera&,
                      const std::vector<std::shared_ptr<LightSource> >&) const {
}

void JokersTrap::depth_test_draw(const Camera&,
                                 std::shared_ptr<ShaderProgram>) const {}