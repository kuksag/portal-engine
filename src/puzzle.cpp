#include "puzzle.h"

#include "portal.h"

JokersTrap::JokersTrap()
    : Drawable(std::make_shared<ShaderProgram>("shaders/light.vertex",
                                               "shaders/light.fragment")) {
    const float SCALE_OBJECT = 0.8;

    patterns.resize(EDGE_NUMBER);

    patterns[0].centroid = new Sphere(glm::vec3(0.0), {0.44, 0.12, 0.78});
    patterns[1].centroid = new Torus(glm::vec3(0.0), {0.16, 0.95, 0.04});
    patterns[2].centroid = new Cone(glm::vec3(0.0), {0.88, 0.57, 0.52});
    patterns[3].centroid = new Cylinder(glm::vec3(0.0), {0.2, 0.03, 0.92});
    patterns[4].centroid = new Cone(glm::vec3(0.0), {0.92, 0.48, 0.16});
    patterns[5].centroid = new Cube(glm::vec3(0.0), {0.2, 0.66, 0.66});

    patterns[3].centroid->scale(glm::vec3(0.6));
    patterns[5].centroid->scale(glm::vec3(0.6));

    for (std::size_t i = 0; i < EDGE_NUMBER; i++) {
        patterns[i].centroid->scale(glm::vec3(SCALE_OBJECT));
        patterns[i].translate(
            glm::vec3(static_cast<float>(i + 1) * MOVE_DISTANCE));
        patterns[i].inverse_rotate();
        base.portals[i].set_destination(&patterns[i].portals[i]);
    }

    patterns[4].portals[4].rotate(M_PI, {1.0, 0.0, 0.0});
    patterns[5].portals[5].rotate(M_PI, {1.0, 0.0, 0.0});

    base.portals[4].set_destination(&patterns[4].portals[4]);
    base.portals[5].set_destination(&patterns[5].portals[5]);
}

JokersTrap::PortalsCube::PortalsCube() : centroid(nullptr) {
    const float BOUND_MATCH = 0.9;

    static const float x_offset[] = {0, 0, 1, -1, 0, 0};
    static const float y_offset[] = {0, 0, 0, 0, 1, -1};
    static const float z_offset[] = {1, -1, 0, 0, 0, 0};

    portals.resize(EDGE_NUMBER);
    for (std::size_t i = 0; i < EDGE_NUMBER; i++)
        portals[i].toggle_draw_bounds();

    for (std::size_t j = 0; j < EDGE_NUMBER; j++) {
        portals[j].translate({x_offset[j], y_offset[j], z_offset[j]});
        portals[j].scale({glm::vec3(BOUND_MATCH)});
        // This scale is needed in order to have a perfect cube
    }

    portals[0].rotate(M_PI, {0.0, 1.0, 0.0});
    portals[2].rotate(-M_PI_2, {0.0, 1.0, 0.0});
    portals[3].rotate(M_PI_2, {0.0, 1.0, 0.0});
    portals[4].rotate(M_PI_2, {1.0, 0.0, 0.0});
    portals[5].rotate(-M_PI_2, {1.0, 0.0, 0.0});
}

void JokersTrap::PortalsCube::inverse_rotate() {
    portals[0].rotate(M_PI, {0.0, 1.0, 0.0});
    portals[1].rotate(M_PI, {0.0, 1.0, 0.0});
    portals[2].rotate(M_PI, {0.0, 1.0, 0.0});
    portals[3].rotate(M_PI, {0.0, 1.0, 0.0});
    portals[4].rotate(M_PI, {1.0, 0.0, 0.0});
    portals[5].rotate(M_PI, {1.0, 0.0, 0.0});
}

void JokersTrap::PortalsCube::translate(glm::vec3 data) {
    for (auto &i : portals) i.translate(data);
    centroid->translate(data);
}

void JokersTrap::draw(const Camera &camera) const {
    std::vector<const Portal *> portals;
    std::vector<const Drawable *> objects;
    // -------------------------------------------------------------------------
    for (std::size_t i = 0; i < EDGE_NUMBER; i++) {
        objects.push_back(patterns[i].centroid);
        for (std::size_t j = 0; j < EDGE_NUMBER; j++) {
            if (i != j) objects.push_back(&patterns[i].portals[j]);
        }
    }
    for (auto &i : base.portals) portals.push_back(&i);
    // -------------------------------------------------------------------------
    auto custom_camera = camera;
    custom_camera.set_view_matrix(camera.get_view_matrix() *
                                  get_model_matrix());
    // -------------------------------------------------------------------------
    render_scene(custom_camera, objects, portals, 0);
}

void JokersTrap::set_light_sources(const std::vector<LightSource> *data) {
    for (auto &i : base.portals) i.set_light_sources(data);
    for (PortalsCube &pattern : patterns)
        pattern.centroid->set_light_sources(data);
}
void JokersTrap::depth_test_draw(
    const Camera &camera, std::shared_ptr<ShaderProgram> depth_sahder) const {}
