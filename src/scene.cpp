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

	for (const auto& i : primitives)
		i->draw(camera, lights);
	for (const auto& i : models)
		i->draw(camera, lights);

	glfwSwapBuffers(window.glfw_window());
}

Scene::prim_ptr Scene::add_primitive(const glm::vec3& position, const glm::vec3& color) {
	prim_ptr res = std::make_shared<Cube>(position, color);
	primitives.push_back(res);
	return res;
}