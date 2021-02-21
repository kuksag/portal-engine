#ifndef PORTAL_ENGINE_DRAWABLE_H
#define PORTAL_ENGINE_DRAWABLE_H

#include "entity.h"

struct Drawable : Entity {
    virtual void draw() = 0;
    //virtual void draw_shape() = 0;
};

#endif    // PORTAL_ENGINE_DRAWABLE_H
