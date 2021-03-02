#include "SceneObjects/portal.h"

#include "camera.h"

Portal::Portal() : bound(4), destination(this) {
    // ---------------------
    // обводка

    for (auto& line : bound) {
        line.scale({1.1, 0.1, 0.1});
    }
    bound[0].translate({0.0, -0.5, 0.0});
    bound[1].translate({0.0, 0.5, 0.0});

    bound[2].translate({-0.5, 0.0, 0.0});
    bound[2].rotate(M_PI_2, {0.0, 0.0, 1.0});

    bound[3].translate({0.5, 0.0, 0.0});
    bound[3].rotate(M_PI_2, {0.0, 0.0, 1.0});

    // ---------------------
    // маячок

    beacon.scale({0.1, 0.1, 0.1});
    beacon.translate({0.0, 0.0, -0.2});

    // ---------------------
    // задняя стенка

    back_plane.translate({0.0, 0.0, 0.09});
}

void Portal::draw(ShaderProgram& shader,
                  const glm::mat4& camera_projection_view) {
    plane.draw(shader, camera_projection_view * get_model_matrix());
}

void Portal::draw_shape(ShaderProgram& shader,
                        const glm::mat4& camera_projection_view) {
    shader.use();

    for (auto& line : bound) {
        glUniform3f(shader.get_uniform_id("extra_color"), 0.3, 0.6, 0.5);
        line.draw(shader, camera_projection_view * get_model_matrix());
        glUniform3f(shader.get_uniform_id("extra_color"), 0.0, 0.0, 0.0);
    }

//    glUniform3f(shader.get_uniform_id("extra_color"), 1.0f, 0.0f, 0.0f);
//    beacon.draw(shader, camera_projection_view * get_model_matrix());
//    glUniform3f(shader.get_uniform_id("extra_color"), 0.0, 0.0, 0.0);

    back_plane.draw(shader, camera_projection_view * get_model_matrix());
}

void Portal::set_destination(Portal* destination_) {
    destination = destination_;
}

Portal* Portal::get_destination() { return destination; }
