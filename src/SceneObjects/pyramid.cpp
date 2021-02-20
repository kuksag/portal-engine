#include "SceneObjects/pyramid.h"

Pyramid::Pyramid() : mesh("Pyramid/pyramid.vertex",
                    "Pyramid/pyramid.fragment") {
    mesh.add_vertices({1.0f, 0.0f, 0.0,
                       0.0f, 1.0f, 0.0,
                       0.0f, 0.0f, 0.0,
                       0.0f, 0.0f, 1.0});

    mesh.add_colors({1.0f, 0.0f, 0.0f,
                     0.0f, 1.0f, 0.0f,
                     0.5f, 0.5f, 0.5f,
                     0.0f, 0.0f, 1.0f});

    mesh.add_indexed_vertices({0, 2, 1, 0, 2, 3, 0, 1, 3, 1, 2, 3});
}

void Pyramid::supply_shader(const glm::vec3 &fancy_color) {
    mesh.shader.use();
    glUniform3f(mesh.shader.get_uniform_id("fancy_color"),
                fancy_color[0], fancy_color[1], fancy_color[2]);
}

void Pyramid::draw(const glm::mat4 &camera_projection_view) {
    mesh.shader.use();
    mesh.bind();

    auto MVP = camera_projection_view * get_model_matrix();

    glUniformMatrix4fv(
        mesh.shader.get_uniform_id("MVP"), 1, GL_FALSE,
        &MVP[0][0]);


    glDrawElements(GL_TRIANGLES, mesh.get_number_of_vertices(),
                   GL_UNSIGNED_INT, nullptr);

    Mesh::unbind();
}

void Pyramid::draw_shape(const glm::mat4 &camera_projection_view) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    draw(camera_projection_view);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}
