#ifndef PORTAL_ENGINE_PORTAL_H
#define PORTAL_ENGINE_PORTAL_H

#include "drawable.h"
#include "mesh.h"

struct Portal : Drawable {
private:
    Mesh mesh;

public:
    Portal();
    void draw(ShaderProgram &shader,
                      const glm::mat4 &camera_projection_view);
    void draw_shape(ShaderProgram &shader,
                            const glm::mat4 &camera_projection_view);
};

#endif    // PORTAL_ENGINE_PORTAL_H
