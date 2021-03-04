#ifndef PORTAL_ENGINE_PORTAL_H
#define PORTAL_ENGINE_PORTAL_H

#include "drawable.h"
#include "primitives.h"

struct Portal : Drawable {
private:
    Plane plane;
    Portal *destination;

public:
    Portal();

    void draw(const Camera &camera) const override;
};

#endif    // PORTAL_ENGINE_PORTAL_H
