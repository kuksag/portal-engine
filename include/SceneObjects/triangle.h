#ifndef PORTAL_ENGINE_TRIANGLE_H
#define PORTAL_ENGINE_TRIANGLE_H

#include <vector>

#include "Texture.h"
#include "drawable.h"
#include "mesh.h"

struct Triangle : Drawable {
private:
    Mesh mesh;
    Texture texture;

public:
    Triangle();

    void draw(ShaderProgram &shader,
              const glm::mat4 &camera_projection_view) override;
    void draw_shape(ShaderProgram &shader,
                    const glm::mat4 &camera_projection_view) override;
};

#endif    // PORTAL_ENGINE_TRIANGLE_H
