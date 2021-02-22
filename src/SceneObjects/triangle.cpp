#include "SceneObjects/triangle.h"
#include "Texture.h"

Triangle::Triangle(): mesh("Triangle/triangle.vertex", "Triangle/triangle.fragment"),
                      texture("res/textures/container.jpeg") {
    mesh.add_vertices({-1.0f, -1.0f, 0.0f,
                       1.0f, -1.0f, 0.0,
                       0.0f, 1.0f, 0.0f});
    mesh.add_indexed_vertices({0, 1, 2});
    mesh.add_textures({0.0f, 0.0f,
                            1.0f, 0.0f,
                            0.5f, 1.0f});
}

void Triangle::draw(const glm::mat4 &camera_projection_view) {
    texture.bind();
    mesh.shader.use();


    auto MVP = camera_projection_view * get_model_matrix();
    glUniformMatrix4fv(
        mesh.shader.get_uniform_id("MVP"), 1, GL_FALSE,
        &MVP[0][0]);

    mesh.bind();
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
    mesh.unbind();

    texture.unbind();
}
void Triangle::draw_shape(const glm::mat4 &camera_projection_view) {
    //do nothing
}



