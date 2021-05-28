#ifndef PORTAL_ENGINE_PUZZLE_H
#define PORTAL_ENGINE_PUZZLE_H

#include <memory>
#include <vector>

#include "drawable.h"

struct Scene;
struct Portal;

struct JokersTrap : Drawable {
private:
    static const std::size_t EDGE_NUMBER = 6;
    const float MOVE_DISTANCE = 100.0F * Settings::Camera::DISPLAY_RANGE_FAR;

    struct PortalsCube : Entity {
        std::vector<std::shared_ptr<Portal>> portals;
        std::shared_ptr<Drawable> centroid;
        PortalsCube(Scene& scene);
        void inverse_rotate();
    };

    inline static std::vector<PortalsCube> patterns;
    PortalsCube base;

public:
    JokersTrap(Scene& scene, const glm::vec3& position,
               std::shared_ptr<ShaderProgram> shader);

    void draw(
        const Camera& camera,
        const std::vector<std::shared_ptr<LightSource>>& ls) const override;
    void depth_test_draw(
        const Camera& camera,
        std::shared_ptr<ShaderProgram> depth_shader) const override;
};

#endif    // PORTAL_ENGINE_PUZZLE_H
