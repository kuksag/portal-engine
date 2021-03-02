#ifndef PORTAL_ENGINE_PLANE_H
#define PORTAL_ENGINE_PLANE_H

#include <mesh.h>
#include "drawable.h"

struct Plane : Drawable {
private:
    Mesh mesh;

public:
    Plane();

    void draw(ShaderProgram &shader, const glm::mat4 &camera_projection_view) override;
    void draw_shape(ShaderProgram &shader,
                    const glm::mat4 &camera_projection_view) override;

};

#endif    // PORTAL_ENGINE_PLANE_H
