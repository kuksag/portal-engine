#ifndef PORTAL_ENGINE_PORTAL_H
#define PORTAL_ENGINE_PORTAL_H

#include <memory>

#include "shader.h"
#include "drawable.h"
#include "primitives.h"
#include "scene.h"


struct Portal : public Drawable {
    // -------------------------------------------------------------------------
    // because we dont want to write bounds into stencil buffer
    // void draw_bounds(const Camera &camera) const;

private:
    std::shared_ptr<Primitive> shape;
    Portal *destination;

    // -------------------------------------------------------------------------
    // debug info:
    // 'beacon' shows direction of portal (turned off by default)
    // 'bounds' show bounds of portal (turned on by default)
    // TODO: make key trigger to turn on debug-info; make flags
    // std::shared_ptr<Primitive> beacon;
    std::vector<std::shared_ptr<Primitive>> bounds;
    // -------------------------------------------------------------------------

    [[nodiscard]] glm::vec3 get_center() const;

public:
    // bool is_draw_bounds = false;

    Portal(Scene* scene, std::shared_ptr<ShaderProgram> shader);

    void translate(const glm::vec3 &data) override;

    // void toggle_draw_bounds();

    // TODO: make protected
    void draw1(const Camera &camera/*, std::shared_ptr<ShaderProgram> depth_shader*/) const;
    void depth_test_draw(
        const Camera& camera,
        std::shared_ptr<ShaderProgram> depth_shader) const override;

    void draw(const Camera& camera, const std::vector< std::shared_ptr<LightSource> >& ls) const override;

    void set_destination(Portal *other);

    friend Camera get_portal_destination_camera(const Camera &camera,
                                                const Portal &portal);

    // friend void draw_portals(const Camera &camera,
    //                          const std::vector<Portal *> &portals,
    //                          bool draw_bounds);

    // void set_light_sources(const std::vector<LightSource> *data) override;
};

#endif    // PORTAL_ENGINE_PORTAL_H
