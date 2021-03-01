#include "SceneObjects/portal.h"

Portal::Portal() : mesh() {

    mesh.add_vertices({
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f});

    mesh.add_colors({
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f});

    mesh.add_indexed_vertices({0, 1, 2, 3, 4, 5});
}

void Portal::draw(ShaderProgram& shader,
                  const glm::mat4& camera_projection_view) {
    shader.use();
    mesh.bind();

    auto MVP = camera_projection_view * get_model_matrix();

    glUniformMatrix4fv(shader.get_uniform_id("MVP"), 1, GL_FALSE,
                       &MVP[0][0]);

    glDrawElements(GL_TRIANGLES, mesh.get_number_of_vertices(), GL_UNSIGNED_INT,
                   nullptr);

    mesh.unbind();
}

void Portal::draw_shape(ShaderProgram& shader,
                        const glm::mat4& camera_projection_view) {}
