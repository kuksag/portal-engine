#include "SceneObjects/cube.h"

Cube::Cube() : mesh("shaders/light.vertex", "shaders/light.fragment") {
    mesh.add_vertices(
        {-0.5f, -0.5f, -0.5f,
         0.5f,  -0.5f, -0.5f,
         0.5f,  0.5f,  -0.5f,
         0.5f,  0.5f,  -0.5f,
         -0.5f, 0.5f,  -0.5f,
         -0.5f, -0.5f, -0.5f,
         -0.5f, -0.5f, 0.5f,
         0.5f,  -0.5f, 0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         -0.5f, 0.5f,  0.5f,
         -0.5f, -0.5f, 0.5f,
         -0.5f, 0.5f,  0.5f,
         -0.5f, 0.5f,  -0.5f,
         -0.5f, -0.5f, -0.5f,
         -0.5f, -0.5f, -0.5f,
         -0.5f, -0.5f, 0.5f,
         -0.5f, 0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  -0.5f,
         0.5f,  -0.5f, -0.5f,
         0.5f,  -0.5f, -0.5f,
         0.5f,  -0.5f, 0.5f,
         0.5f,  0.5f,  0.5f,
         -0.5f, -0.5f, -0.5f,
         0.5f,  -0.5f, -0.5f,
         0.5f,  -0.5f, 0.5f,
         0.5f,  -0.5f, 0.5f,
         -0.5f, -0.5f, 0.5f,
         -0.5f, -0.5f, -0.5f,
         -0.5f, 0.5f,  -0.5f,
         0.5f,  0.5f,  -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         -0.5f, 0.5f,  0.5f,
         -0.5f, 0.5f,  -0.5f});
    mesh.add_normals(
        { 0.0f,  0.0f,  -1.0f,
          0.0f,  0.0f,  -1.0f,
          0.0f,  0.0f,  -1.0f,
          0.0f,  0.0f,  -1.0f,
          0.0f,  0.0f,  -1.0f,
          0.0f,  0.0f,  -1.0f,
          0.0f,  0.0f,  1.0f,
          0.0f,  0.0f,  1.0f,
          0.0f,  0.0f,  1.0f,
          0.0f,  0.0f,  1.0f,
          0.0f,  0.0f,  1.0f,
          0.0f,  0.0f,  1.0f,
          -1.0f, 0.0f,  0.0f,
          -1.0f, 0.0f,  0.0f,
          -1.0f, 0.0f,  0.0f,
          -1.0f, 0.0f,  0.0f,
          -1.0f, 0.0f,  0.0f,
          -1.0f, 0.0f,  0.0f,
          1.0f,  0.0f,  0.0f,
          1.0f,  0.0f,  0.0f,
          1.0f,  0.0f,  0.0f,
          1.0f,  0.0f,  0.0f,
          1.0f,  0.0f,  0.0f,
          1.0f,  0.0f,  0.0f,
          0.0f,  -1.0f, 0.0f,
          0.0f,  -1.0f, 0.0f,
          0.0f,  -1.0f, 0.0f,
          0.0f,  -1.0f, 0.0f,
          0.0f,  -1.0f, 0.0f,
          0.0f,  -1.0f, 0.0f,
          0.0f,  1.0f,  0.0f,
          0.0f,  1.0f,  0.0f,
          0.0f,  1.0f,  0.0f,
          0.0f,  1.0f,  0.0f,
          0.0f,  1.0f,  0.0f,
          0.0f,  1.0f,  0.0f});
    std::vector<int> id(36);
    for (int i = 0; i < 36; i++) id[i] = i;
    mesh.add_indexed_vertices(id);
}

void Cube::set_light_position(glm::vec3 light_position) {
    mesh.shader.use();
    glUniform3f(mesh.shader.get_uniform_id("light_pos"),
                light_position[0], light_position[1], light_position[2]);
}

void Cube::set_light_color(glm::vec3 light_color) {
    mesh.shader.use();
    glUniform3f(mesh.shader.get_uniform_id("light_color"),
                light_color[0], light_color[1], light_color[2]);
}

void Cube::set_shader_model_matrix() {
}

void Cube::set_camera_pos(glm::vec3 camera_pos) {
    mesh.shader.use();
    glUniform3f(mesh.shader.get_uniform_id("camera_pos"),
                camera_pos[0], camera_pos[1], camera_pos[2]);
}

void Cube::set_translate(glm::vec3 translate) {
    mesh.shader.use();
    glUniform3f(mesh.shader.get_uniform_id("trans"),
                translate[0], translate[1], translate[2]);
}


void Cube::draw(const glm::mat4& camera_projection_view) {
    mesh.shader.use();
    mesh.bind();

    auto MVP = camera_projection_view * get_model_matrix();

    glUniformMatrix4fv(mesh.shader.get_uniform_id("MVP"),
                       1,
                       GL_FALSE,
                       &MVP[0][0]);

    glDrawElements(GL_TRIANGLES,
                   mesh.get_number_of_vertices(),
                   GL_UNSIGNED_INT,
                   nullptr);

    mesh.unbind();
}

void Cube::draw_shape(const glm::mat4& camera_projection_view) {}
