#ifndef PORTAL_ENGINE_CUBE_H
#define PORTAL_ENGINE_CUBE_H

#include <glm/glm.hpp>
#include <memory>

#include "model.h"
#include "shader.h"

class Cube : public Model
{
private:

	glm::vec3 color;

public:

	Cube(const glm::vec3& position = {0, 0, 0}, const glm::vec3& color = {1, 1, 1});
	void set_color(const glm::vec3& color);
	glm::vec3 get_color() const;
};

#endif    // PORTAL_ENGINE_CUBE_H