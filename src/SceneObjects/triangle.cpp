#include "SceneObjects/triangle.h"

#include "Texture.h"

Triangle::Triangle() : mesh(), texture("res/textures/container.jpeg") {
    mesh.add_vertices({-1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0, 0.0f, 1.0f, 0.0f});
    mesh.add_indexed_vertices({0, 1, 2});
    mesh.add_textures({0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f});
}

void Triangle::draw(ShaderProgram &shader,
                    const glm::mat4 &camera_projection_view) {
    texture.bind();
    shader.use();

    auto MVP = camera_projection_view * get_model_matrix();
    glUniformMatrix4fv(shader.get_uniform_id("MVP"), 1, GL_FALSE, &MVP[0][0]);

    mesh.bind();
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    mesh.unbind();

    texture.unbind();
}

[[maybe_unused]] void Triangle::draw_shape(
    ShaderProgram &shader, const glm::mat4 &camera_projection_view) {
    // do nothing
}
