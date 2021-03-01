#ifndef PORTAL_ENGINE_CUBE_H
#define PORTAL_ENGINE_CUBE_H

#include "drawable.h"
#include "mesh.h"

struct Cube : Drawable {
private:
    Mesh mesh;

public:
    Cube();

    // TODO: theres no need of this functions; we have to call directly shader
    void set_light_position(ShaderProgram &shader, glm::vec3 light_position);
    void set_light_color(ShaderProgram &shader, glm::vec3 light_color);
    void set_shader_model_matrix();
    void set_camera_pos(ShaderProgram &shader, glm::vec3 camera_pos);
    void set_translate(ShaderProgram &shader, glm::vec3 translate);

    void draw(ShaderProgram &shader,
              const glm::mat4 &camera_projection_view) override;
    void draw_shape(ShaderProgram &shader,
                    const glm::mat4 &camera_projection_view) override;
};

#endif    // PORTAL_ENGINE_CUBE_H
