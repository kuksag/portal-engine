#ifndef PORTAL_ENGINE_PORTAL_H
#define PORTAL_ENGINE_PORTAL_H

#include "drawable.h"
#include "primitives.h"

struct Portal : Drawable {
private:
    Plane shape;
    Portal *destination;

    // -------------------------------------------------------------------------
    // debug info:
    // 'beacon' shows direction of portal (turned off by default)
    // 'bounds' show bounds of portal (turned on by default)
    // TODO: make key trigger to turn on debug-info; make flags
    Cone beacon;
    std::vector<Cube> bounds;
    // -------------------------------------------------------------------------
    // because we dont want to write bounds into stencil buffer
    void draw_bounds(const Camera &camera) const;
    // -------------------------------------------------------------------------

    [[nodiscard]] glm::vec3 get_center() const;

public:
    Portal();

    // TODO: make protected
    void draw(const Camera &camera) const override;

    void set_destination(Portal *other);

    friend Camera get_portal_destination_camera(const Camera &camera,
                                                const Portal &portal);

    friend void render_scene(const Camera &camera,
                             const std::vector<Drawable *> &objects,
                             const std::vector<Portal *> &portals,
                             int recursion_level);

    friend void draw_portals(const Camera &camera,
                             const std::vector<Portal *> &portals,
                             bool draw_bounds);
};

void render_scene(const Camera &camera, const std::vector<Drawable *> &objects,
                  const std::vector<Portal *> &portals);


#endif    // PORTAL_ENGINE_PORTAL_H
