#include "portal.h"

Portal::Portal()
    : Drawable(std::make_shared<ShaderProgram>("shaders/color.vertex",
                                               "shaders/color.fragment")),
      destination(this) {
    // -------------------------------------------------------------------------
    shape.scale({1.0, 1.0, 0.1});
    // -------------------------------------------------------------------------
    beacon.scale({0.2, 0.2, 0.2});
    beacon.set_color({0.0, 1.0, 0.0});
    beacon.rotate(M_PI_2, {0.0, 1.0, 0.0});
    beacon.rotate(M_PI_2, {0.0, 0.0, 1.0});
    beacon.translate({0.0, 0.0, 0.5});
    // -------------------------------------------------------------------------
    bounds.resize(4);
    for (auto& bound : bounds) {
        bound.scale({1.2, 0.1, 0.1});
        bound.set_color({0.0, 0.0, 1.0});
    }
    bounds[0].translate({0.0, -1.1, 0.0});
    bounds[1].translate({0.0, 1.1, 0.0});

    bounds[2].translate({-1.1, 0.0, 0.0});
    bounds[2].rotate(M_PI_2, {0.0, 0.0, 1.0});

    bounds[3].translate({1.1, 0.0, 0.0});
    bounds[3].rotate(M_PI_2, {0.0, 0.0, 1.0});
    // -------------------------------------------------------------------------
}

void Portal::draw(const Camera& camera) const {
    auto custom_camera = camera;
    custom_camera.set_view_matrix(camera.get_view_matrix() *
                                  get_model_matrix());
    shape.draw(custom_camera);
    beacon.draw(custom_camera);
    for (auto& bound : bounds) bound.draw(custom_camera);
}
