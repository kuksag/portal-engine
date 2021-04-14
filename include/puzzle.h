#ifndef PORTAL_ENGINE_PUZZLE_H
#define PORTAL_ENGINE_PUZZLE_H

#include "portal.h"

struct PortalsCube {
    std::vector<Portal> portals;
    Drawable *centroid;
    PortalsCube();
    void inverse_rotate();

    void translate(glm::vec3 data);
    void scale(glm::vec3 data);
};


struct JokersTrap {
    const std::size_t EDGE_NUMBER = 6;
    const float MOVE_DISTANCE = 100.0F * Settings::Camera::DISPLAY_RANGE_FAR;
    std::vector<PortalsCube> patterns;
    PortalsCube base;

    JokersTrap();
};


#endif    // PORTAL_ENGINE_PUZZLE_H
