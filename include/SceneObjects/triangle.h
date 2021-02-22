#ifndef PORTAL_ENGINE_TRIANGLE_H
#define PORTAL_ENGINE_TRIANGLE_H

#include "drawable.h"

#include "mesh.h"
#include <vector>
#include "Texture.h"

struct Triangle : Drawable {
private:
    Mesh mesh;
    Texture texture;
public:
    Triangle();

    void draw(const glm::mat4 &camera_projection_view) override;
    void draw_shape(const glm::mat4 &camera_projection_view) override;
};


#endif    // PORTAL_ENGINE_TRIANGLE_H
