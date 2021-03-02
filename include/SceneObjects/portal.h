#ifndef PORTAL_ENGINE_PORTAL_H
#define PORTAL_ENGINE_PORTAL_H

#include "camera.h"
#include "drawable.h"
#include "mesh.h"
#include "SceneObjects/cube.h"
#include "SceneObjects/plane.h"

struct Portal : Drawable {
private:
    std::vector<Cube> bound;
    Cube beacon;
    Portal *destination;
    Plane plane, back_plane;

public:
    Portal();


    void draw(ShaderProgram &shader, const glm::mat4 &camera_projection_view) override;
    void draw_shape(ShaderProgram &shader,
                    const glm::mat4 &camera_projection_view) override;
    void set_destination(Portal *destination_);
    Portal *get_destination();
};

#endif    // PORTAL_ENGINE_PORTAL_H
