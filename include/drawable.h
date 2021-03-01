#ifndef PORTAL_ENGINE_DRAWABLE_H
#define PORTAL_ENGINE_DRAWABLE_H

#include "entity.h"
#include "shader.h"

struct Drawable : Entity {
    virtual void draw(ShaderProgram &shader,
                      const glm::mat4 &camera_projection_view) = 0;
    virtual void draw_shape(ShaderProgram &shader,
                            const glm::mat4 &camera_projection_view) = 0;
};

#endif    // PORTAL_ENGINE_DRAWABLE_H
