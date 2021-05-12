#ifndef PORTAL_ENGINE_SCENE_H
#define PORTAL_ENGINE_SCENE_H

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

#include "window.h"
#include "camera.h"
#include "model.h"
#include "primitives.h"
#include "light_source.h"

struct Portal;

class Scene {
private:

	Window& window;
	Camera& camera;//TODO: make own camera

	std::vector< std::vector<std::shared_ptr<Primitive>> > primitives;
	std::vector<std::shared_ptr<LightSource>> lights;
	std::unordered_map<std::string, std::vector<std::shared_ptr<Model>>> models;
	std::vector< std::shared_ptr<Portal> > portals;

	std::shared_ptr<ShaderProgram> lighted_shader;

	glm::vec3 bg_color = {1, 1, 1};

public:

	Scene(Window& window, Camera& camera);

	void draw() const;

	template<class Type>
	std::shared_ptr<Primitive> add_primitive(const glm::vec3& position = {0, 0, 0}, const glm::vec3& color = {1, 1, 1});
	
	std::shared_ptr<Model> add_model(const std::string& path, const glm::vec3& position = {0, 0, 0});

	std::shared_ptr<LightSource> add_light(const glm::vec3& position = {0, 0, 0}, const glm::vec3& color = {1, 1, 1});

	std::shared_ptr<Portal> add_portal(const glm::vec3& position = {0, 0, 0});

	void set_bg_color(const glm::vec3& color);

};

namespace {
template<class Type> struct PrimId { static const int id = -1; };
template<> struct PrimId<Sphere> { static const int id = 0; };
template<> struct PrimId<Cube> { static const int id = 1; };
template<> struct PrimId<Plane> { static const int id = 2; };
template<> struct PrimId<Cylinder> { static const int id = 3; };
template<> struct PrimId<Torus> { static const int id = 4; };
template<> struct PrimId<Cone> { static const int id = 5; };
} //namespace

template<class Type>
std::shared_ptr<Primitive> Scene::add_primitive(const glm::vec3& position, const glm::vec3& color) {
	static_assert(PrimId<Type>::id != -1);
	int id = PrimId<Type>::id;
	if (primitives[id].empty())
		primitives[id].push_back(std::make_shared<Type>(lighted_shader, true));
	primitives[id].push_back(std::make_shared<Type>(lighted_shader, false, position, color));
	return primitives[id].back();
}

#endif    // PORTAL_ENGINE_SCENE_H