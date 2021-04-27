#include "scene.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "settings.h"

#include "window.h"
#include "portal.h"
#include "camera.h"

Scene::Scene(Window& window, Camera& camera) : window(window), camera(camera), primitives(PORTAL_ENGINE_PRIMITIVES_COUNT), lighted_shader(new ShaderProgram("shaders/light.vertex", "shaders/light.fragment")) {
	glClearColor(bg_color.x, bg_color.y, bg_color.z, 0.0f);
	glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void Scene::draw() const {
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto& prim : primitives) {
		for (unsigned i=1; i < prim.size(); ++i) {
			prim[0]->move_to(prim[i]);
			prim[0]->set_color(prim[i]->get_color());
			prim[0]->draw(camera, lights);
		}
	}

	for (auto i : models) {
		for (unsigned j=1; j < i.second.size(); ++j) {
			i.second[0]->move_to(i.second[j]);
			i.second[0]->draw(camera, lights);
		}
	}

	for (auto& ls : lights) {
		ls->start_depth_test();
		for (const auto& i : models)
			for (unsigned j=1; j < i.second.size(); ++j)
				ls->gen_depth_map(dynamic_cast<Drawable*>(i.second[j].get()));
		for (const auto& j : primitives)
			for (const auto& i : j)
				ls->gen_depth_map(dynamic_cast<Drawable*>(i.get()));
		ls->finish_depth_test();
	}
    glViewport(0, 0, Settings::Window::WIDTH, Settings::Window::HEIGHT); //TODO

	glfwSwapBuffers(window.glfw_window());
}

std::shared_ptr<Model> Scene::add_model(const std::string& path, const glm::vec3& position) {
	if (models[path].empty())
		models[path].push_back(std::make_shared<Model>(path, lighted_shader, true));
	models[path].push_back(std::make_shared<Model>(path, lighted_shader, false));
	return models[path].back();
}

std::shared_ptr<LightSource> Scene::add_light(const glm::vec3& position, const glm::vec3& color) {
	lights.push_back(std::make_shared<LightSource>(position, color));
	return lights.back();
}

// std::shared_ptr<Portal> Scene::add_portal() {

// }


void Scene::set_bg_color(const glm::vec3& color) {
	bg_color = color;
	glClearColor(bg_color.x, bg_color.y, bg_color.z, 0.0f);
}