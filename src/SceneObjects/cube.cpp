#include "SceneObjects/cube.h"

Cube::Cube() : mesh() {
    mesh.add_vertices(
        {-0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,
         0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
         0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,
         -0.5f, 0.5f,  0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f,
         0.5f,  -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
         0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  -0.5f,
         0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,
         0.5f,  0.5f,  -0.5f, -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f,
         0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f, -0.5f, 0.5f,  -0.5f, -0.5f, -0.5f,
         -0.5f, 0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  -0.5f, 0.5f,  0.5f,  -0.5f, 0.5f,  -0.5f});
    mesh.add_normals(
        {0.0f, 0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
         0.0f, -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,
         1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.0f, 0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  -1.0f, 0.0f,  0.0f,  -1.0f,
         0.0f, 0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
         0.0f, -1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f,
         0.0f, 0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,  0.0f,  -1.0f, 0.0f,
         0.0f, 1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         1.0f, 0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f});
    std::vector<int> id(36);
    for (int i = 0; i < 36; i++) id[i] = i;
    mesh.add_indexed_vertices(id);
}

void Cube::set_light_position(ShaderProgram &shader, glm::vec3 light_position) {
    shader.use();
    glUniform3f(shader.get_uniform_id("light_pos"), light_position[0],
                light_position[1], light_position[2]);
}

void Cube::set_light_color(ShaderProgram &shader, glm::vec3 light_color) {
    shader.use();
    glUniform3f(shader.get_uniform_id("light_color"), light_color[0],
                light_color[1], light_color[2]);
}

void Cube::set_shader_model_matrix() {}

void Cube::set_camera_pos(ShaderProgram &shader, glm::vec3 camera_pos) {
    shader.use();
    glUniform3f(shader.get_uniform_id("camera_pos"), camera_pos[0],
                camera_pos[1], camera_pos[2]);
}

void Cube::set_translate(ShaderProgram &shader, glm::vec3 translate) {
    shader.use();
    glUniform3f(shader.get_uniform_id("trans"), translate[0], translate[1],
                translate[2]);
}

void Cube::draw(ShaderProgram &shader,
                const glm::mat4 &camera_projection_view) {
    shader.use();
    mesh.bind();

    auto MVP = camera_projection_view * get_model_matrix();

    glUniformMatrix4fv(shader.get_uniform_id("MVP"), 1, GL_FALSE, &MVP[0][0]);

    glDrawElements(GL_TRIANGLES, mesh.get_number_of_vertices(), GL_UNSIGNED_INT,
                   nullptr);

    mesh.unbind();
}

void Cube::draw_shape(ShaderProgram &shader,
                      const glm::mat4 &camera_projection_view) {}
