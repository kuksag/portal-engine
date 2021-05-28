#ifndef PORTAL_ENGINE_PUZZLE_H
#define PORTAL_ENGINE_PUZZLE_H

#include "portal.h"

struct JokersTrap : Drawable {
private:
    static const std::size_t EDGE_NUMBER = 6;
    const float MOVE_DISTANCE = 100.0F * Settings::Camera::DISPLAY_RANGE_FAR;

    struct PortalsCube : Entity {
        std::vector<std::shared_ptr<Portal>> portals;
        std::shared_ptr<Drawable> centroid;
        PortalsCube(Scene& scene);
        void inverse_rotate();

        // void translate(const glm::vec3 &data) override;
        // void rotate(float angle, const glm::vec3 &data) override;
        // void scale(const glm::vec3 &data) override;
    };

    inline static std::vector<PortalsCube> patterns;
    PortalsCube base;

public:
    JokersTrap(Scene& scene);

    void draw(const Camera& camera, const std::vector< std::shared_ptr<LightSource> >& ls) const override;
    void depth_test_draw(
        const Camera &camera,
        std::shared_ptr<ShaderProgram> depth_shader) const override;
    // void set_light_sources(const std::vector<LightSource> *data) override;

    // void translate(const glm::vec3 &data) override;
    // void rotate(float angle, const glm::vec3 &data) override;
    // void scale(const glm::vec3 &data) override;
};

#endif    // PORTAL_ENGINE_PUZZLE_H
