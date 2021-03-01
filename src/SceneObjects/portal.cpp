#include "SceneObjects/portal.h"


#include "camera.h"

Portal::Portal() : mesh() {
    std::vector<float> data = {-0.5f, -0.5f, 0.0f, 0.5f,  -0.5f, 0.0f,
                               0.5f,  0.5f,  0.0f, 0.5f,  0.5f,  0.0f,
                               -0.5f, 0.5f,  0.0f, -0.5f, -0.5f, 0.0f};

    mesh.add_vertices(data);

    mesh.add_colors({0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                     0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});

    mesh.add_indexed_vertices({0, 1, 2, 3, 4, 5});

    glm::vec3 center;
    for (int i = 0; i < data.size(); i += 3) {
        for (int j = 0; j < 3; j++) center[j] += data[i + j];
    }
    for (int i = 0; i < 3; i++) center[i] /= 1.0 * data.size();
    set_center(center);
}

void Portal::draw(ShaderProgram& shader,
                  const glm::mat4& camera_projection_view) {
    shader.use();
    mesh.bind();

    auto MVP = camera_projection_view * get_model_matrix();

    glUniformMatrix4fv(shader.get_uniform_id("MVP"), 1, GL_FALSE, &MVP[0][0]);

    glDrawElements(GL_TRIANGLES, mesh.get_number_of_vertices(), GL_UNSIGNED_INT,
                   nullptr);

    mesh.unbind();
}



void Portal::draw_shape(ShaderProgram& shader,
                        const glm::mat4& camera_projection_view) {}

void Portal::set_destination(Portal* destination_) {
    destination = destination_;
}

Portal* Portal::get_destination() { return destination; }
