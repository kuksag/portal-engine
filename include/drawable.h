#ifndef PORTAL_ENGINE_DRAWABLE_H
#define PORTAL_ENGINE_DRAWABLE_H

#include <vector>

#include "entity.h"
#include "mesh.h"
#include "camera.h"

class Drawable : public Entity {
public:

    virtual void draw(const Camera& camera) const = 0;
};

#endif    // PORTAL_ENGINE_DRAWABLE_H
