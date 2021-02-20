#ifndef PORTAL_ENGINE_PYRAMID_H
#define PORTAL_ENGINE_PYRAMID_H

#include "drawable.h"

#include "mesh.h"
#include <vector>

struct Pyramid : Drawable {
private:
    Mesh mesh;

public:
    Pyramid();

    void supply_shader(const glm::vec3 &fancy_color);

    void draw(const glm::mat4 &camera_projection_view) override;
    void draw_shape(const glm::mat4 &camera_projection_view) override;
};

#endif    // PORTAL_ENGINE_PYRAMID_H
