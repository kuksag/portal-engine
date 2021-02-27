#include "light_source.h"
#include "entity.h"
#include <utility>

glm::vec3 LightSource::get_color() const {
    return color;
}

glm::vec3 LightSource::get_pos() const {
    return pos;
}

LightSource::LightSource(glm::vec3 pos_, glm::vec3 color_, std::shared_ptr<ShaderProgram> shader_): pos(pos_),
                                                                                                    color(color_),
                                                                                                    shader(std::move(shader_)) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexes.size(), &vertexes[0], GL_STATIC_DRAW);

    glBindVertexArray(VAO);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
    glBindVertexArray(0);
}

void LightSource::draw(const Camera &camera) {
    shader->use();
    auto MVP = camera.get_projection_matrix() * camera.get_view_matrix() * get_model_matrix();
    glUniformMatrix4fv(shader->get_uniform_id("MVP"), 1, GL_FALSE, &MVP[0][0]);
    glUniform3f(shader->get_uniform_id("light_color"), color.x, color.y, color.z);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0,3);
    glBindVertexArray(0);
}

