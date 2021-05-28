#ifndef PORTAL_ENGINE_SCENE_H
#define PORTAL_ENGINE_SCENE_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "camera.h"
#include "controls.h"
#include "light_source.h"
#include "model.h"
#include "portal_gun.h"
#include "primitives.h"
#include "puzzle.h"
#include "window.h"

struct Portal;

struct PairingPortals {
private:
    std::shared_ptr<Portal> first = nullptr;
    std::shared_ptr<Portal> second = nullptr;

public:
    void set_portals(std::shared_ptr<Portal> first_,
                     std::shared_ptr<Portal> second_);
    void replace_portal(glm::mat4 translation_matrix,
                        glm::mat4 rotation_matrix = glm::mat4());
    [[nodiscard]] glm::vec3 get_first_portal_normal() const;
    void move_first_portal(glm::mat4 translation_matrix,
                           glm::mat4 rotation_matrix = glm::mat4());
};

class Scene {
private:
    Window& window;
    Camera& camera;    // TODO: make own camera
    Controller& controller;

    std::vector<std::vector<std::shared_ptr<Primitive>>> primitives;
    std::vector<std::shared_ptr<LightSource>> lights;
    std::unordered_map<std::string, std::vector<std::shared_ptr<Model>>> models;
    std::vector<std::shared_ptr<Portal>> portals;
    std::vector<std::shared_ptr<Bullet>> bullets;
    std::shared_ptr<ShaderProgram> lighted_shader;

    PortalGun portal_gun;
    PairingPortals player_portals;

    glm::vec3 bg_color = {1, 1, 1};

    void render_scene(
        const Camera& camera, int recursion_level = 0,
        std::function<bool(const Entity*)> is_visible = [](const Entity*) {
            return true;
        }) const;

public:
    Scene(Window& window, Camera& camera, Controller& controller);

    void draw() const;
    void update();

    template <class Type>
    std::shared_ptr<Primitive> add_primitive(const glm::vec3& position = {0, 0,
                                                                          0},
                                             const glm::vec3& color = {1, 1,
                                                                       1});

    std::shared_ptr<Model> add_model(const std::string& path,
                                     const glm::vec3& position = {0, 0, 0});

    std::shared_ptr<LightSource> add_light(const glm::vec3& position = {0, 0,
                                                                        0},
                                           const glm::vec3& color = {1, 1, 1},
                                           float intensity = 0.3f,
                                           bool is_shadowed = false);

    std::shared_ptr<Portal> add_portal(const glm::vec3& position = {0, 0, 0});

    std::shared_ptr<Bullet> add_bullet(const glm::vec3& start_point,
                                       const glm::vec3& direction);

    std::shared_ptr<JokersTrap> add_jokers_trap(const glm::vec3& position = {
                                                    0, 0, 0});

    void set_bg_color(const glm::vec3& color);
};

namespace {
template <class Type>
struct PrimId {
    static const int id = -1;
};
template <>
struct PrimId<Sphere> {
    static const int id = 0;
};
template <>
struct PrimId<Cube> {
    static const int id = 1;
};
template <>
struct PrimId<Plane> {
    static const int id = 2;
};
template <>
struct PrimId<Cylinder> {
    static const int id = 3;
};
template <>
struct PrimId<Torus> {
    static const int id = 4;
};
template <>
struct PrimId<Cone> {
    static const int id = 5;
};
}    // namespace

template <class Type>
std::shared_ptr<Primitive> Scene::add_primitive(const glm::vec3& position,
                                                const glm::vec3& color) {
    static_assert(PrimId<Type>::id != -1);
    int id = PrimId<Type>::id;
    if (primitives[id].empty())
        primitives[id].push_back(std::make_shared<Type>(lighted_shader, true));
    primitives[id].push_back(
        std::make_shared<Type>(lighted_shader, true, position, color));
    return primitives[id].back();
}

#endif    // PORTAL_ENGINE_SCENE_H
