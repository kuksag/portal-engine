#include "cube.h"
#include "shader.h"

#include <glm/glm.hpp>

Cube::Cube(const glm::vec3& position, const glm::vec3& color) : Model("res/models/primitives/cube.obj", std::make_shared<ShaderProgram>("shaders/color.vertex", "shaders/color.fragment")) {
	translate(position);
	set_color(color);
}


void Cube::set_color(const glm::vec3& color) {
	this->color = color;
	shader->use();
	glUniform4f(shader->get_uniform_id("color"), color.x, color.y, color.z, 1);
}
glm::vec3 Cube::get_color() const {
	return color;
}