#ifndef PORTAL_ENGINE_PYRAMID_H
#define PORTAL_ENGINE_PYRAMID_H

#include <vector>

#include "drawable.h"
#include "mesh.h"

struct Pyramid : Drawable {
private:
    Mesh mesh;

public:
    Pyramid();

    static void supply_shader(ShaderProgram &shader, const glm::vec3 &color);

    void draw(ShaderProgram &shader,
              const glm::mat4 &camera_projection_view) override;
    void draw_shape(ShaderProgram &shader,
                    const glm::mat4 &camera_projection_view) override;

};

#endif    // PORTAL_ENGINE_PYRAMID_H
