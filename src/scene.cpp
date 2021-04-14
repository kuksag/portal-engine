#include "scene.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "window.h"
#include "camera.h"

Scene::Scene(Window& window, Camera& camera) : window(window), camera(camera) {

}

void Scene::draw() const {
	glClearColor(0.3f, 0.3f, 0.6f, 0.0f); //TODO: setColor
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (unsigned i=1; i < cubes.size(); ++i)
	{
		cubes[0]->move_to(cubes[i]);
		cubes[0]->draw(camera, lights);
	}
	// for (const auto& i : models)
	// 	i->draw(camera, lights);

	glfwSwapBuffers(window.glfw_window());
}

Scene::prim_ptr Scene::add_cube(const glm::vec3& position, const glm::vec3& color) {
	if (cubes.empty())
		cubes.push_back(std::make_shared<Cube>(true));
	prim_ptr res = std::make_shared<Cube>(false, position, color);
	cubes.push_back(res);
	return res;
}