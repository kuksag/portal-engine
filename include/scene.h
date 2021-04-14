#ifndef PORTAL_ENGINE_SCENE_H
#define PORTAL_ENGINE_SCENE_H

#include <vector>
#include <string>
#include <memory>

#include "window.h"
#include "camera.h"
#include "model.h"
#include "primitives.h"
#include "light_source.h"

class Scene {
	using prim_ptr = std::shared_ptr<Primitive>;
	using model_ptr = std::shared_ptr<Model>;
	using light_ptr = std::shared_ptr<LightSource>;
private:

	Window& window;
	Camera& camera;//TODO: make own camera

	std::vector<prim_ptr> cubes;
	std::vector<model_ptr> models;
	std::vector<LightSource> lights; //TODO: light_ptr

public:

	Scene(Window& window, Camera& camera);

	void draw() const;

	prim_ptr add_cube(const glm::vec3& position = {0, 0, 0}, const glm::vec3& color = {1, 1, 1});
	// model_ptr add_model(const std::string& path, const glm::vec3& position = {0, 0, 0});

};


#endif    // PORTAL_ENGINE_SCENE_H