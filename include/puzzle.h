#ifndef PORTAL_ENGINE_PUZZLE_H
#define PORTAL_ENGINE_PUZZLE_H

#include "portal.h"

struct JokersTrap {
    const std::size_t EDGE_NUMBER = 6;
    const float MOVE_DISTANCE = 10.0F * Settings::Camera::DISPLAY_RANGE_FAR;
//    const float MOVE_DISTANCE = 10.0F;

    std::vector<std::vector<Portal *>> portals;
    std::vector<Drawable *> objects;

    void init_portals();
    void init_objects();

    JokersTrap();
};


#endif    // PORTAL_ENGINE_PUZZLE_H
