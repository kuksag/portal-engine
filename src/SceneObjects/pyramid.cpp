#include "SceneObjects/pyramid.h"

#include <iostream>

Pyramid::Pyramid() : mesh() {
    mesh.add_vertices(
        {1.0f, 0.0f, 0.0, 0.0f, 1.0f, 0.0, 0.0f, 0.0f, 0.0, 0.0f, 0.0f, 1.0});

    mesh.add_colors({1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.5f, 0.0f,
                     0.0f, 1.0f});

    mesh.add_indexed_vertices({0, 2, 1, 0, 2, 3, 0, 1, 3, 1, 2, 3});

    set_center({0.5, 0.5, 0.5});
}

void Pyramid::supply_shader(ShaderProgram &shader, const glm::vec3 &color) {
    shader.use();
    glUniform3f(shader.get_uniform_id("fancy_color"), color[0], color[1],
                color[2]);
}

void Pyramid::draw(ShaderProgram &shader,
                   const glm::mat4 &camera_projection_view) {
    shader.use();
    mesh.bind();

    auto MVP = camera_projection_view * get_model_matrix();

    glUniformMatrix4fv(shader.get_uniform_id("MVP"), 1, GL_FALSE,
                       &MVP[0][0]);

    glDrawElements(GL_TRIANGLES, mesh.get_number_of_vertices(), GL_UNSIGNED_INT,
                   nullptr);

    mesh.unbind();
}

void Pyramid::draw_shape(ShaderProgram &shader,
                         const glm::mat4 &camera_projection_view) {
    supply_shader(shader, {0.0f, 0.0f, 0.0f});

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    draw(shader, camera_projection_view);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
