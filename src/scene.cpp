#include "scene.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <thread>

#include "camera.h"
#include "controls.h"
#include "portal.h"
#include "settings.h"
#include "window.h"

Scene::Scene(Window& window, Camera& camera, Controller& controller)
    : window(window),
      camera(camera),
      controller(controller),
      primitives(PORTAL_ENGINE_PRIMITIVES_COUNT),
      lighted_shader(
          new ShaderProgram("shaders/light.vertex", "shaders/light.fragment")),
      portal_gun(*this, camera) {
    glClearColor(bg_color.x, bg_color.y, bg_color.z, 0.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    auto p1 = add_portal({0, 1.2, 0});
    auto p2 = add_portal({0, 1.2, 5});
    p2->translate({5, 4, 5});
    p2->rotate(-M_PI_4, {1, 1, 0.5});
    player_portals.set_portals(p1, p2);
}

void Scene::draw() const {
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilMask(0xFF);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto& ls : lights) {
        ls->start_depth_test();
        for (const auto& i : models)
            for (unsigned j = 1; j < i.second.size(); ++j)
                ls->gen_depth_map(dynamic_cast<Drawable*>(i.second[j].get()));
        for (const auto& j : primitives)
            for (const auto& i : j)
                if (i->is_visible())
                    ls->gen_depth_map(dynamic_cast<Drawable*>(i.get()));
        ls->finish_depth_test();
    }
    glViewport(0, 0, Settings::Window::WIDTH,
               Settings::Window::HEIGHT);    // TODO

    render_scene(camera, 0);

    glfwSwapBuffers(window.glfw_window());
}

std::shared_ptr<Model> Scene::add_model(const std::string& path,
                                        const glm::vec3& position) {
    if (models[path].empty())
        models[path].push_back(
            std::make_shared<Model>(path, lighted_shader, true));
    models[path].push_back(
        std::make_shared<Model>(path, lighted_shader, false));
    models[path].back()->translate(position);
    return models[path].back();
}

std::shared_ptr<LightSource> Scene::add_light(const glm::vec3& position,
                                              const glm::vec3& color,
                                              float intensity,
                                              bool is_shadowed) {
    lights.push_back(
        std::make_shared<LightSource>(position, color, intensity, is_shadowed));
    return lights.back();
}

std::shared_ptr<Portal> Scene::add_portal(const glm::vec3& position) {
    portals.push_back(std::make_shared<Portal>(this, lighted_shader));
    portals.back()->translate(position);
    return portals.back();
}

std::shared_ptr<JokersTrap> Scene::add_jokers_trap(const glm::vec3& position) {
    return std::make_shared<JokersTrap>(*this, position, lighted_shader);
}

std::shared_ptr<Bullet> Scene::add_bullet(const glm::vec3& start_point,
                                          const glm::vec3& direction) {
    auto ball = add_primitive<Sphere>({0, 0, 0}, Bullet::Bullet_Color);
    bullets.push_back(
        std::shared_ptr<Bullet>(new Bullet(start_point, direction, ball)));
    return bullets.back();
}

void Scene::set_bg_color(const glm::vec3& color) {
    bg_color = color;
    glClearColor(bg_color.x, bg_color.y, bg_color.z, 0.0f);
}

void Scene::update() {
    if (controller.is_enter_pressed()) {
        portal_gun.launch_bullet();
    }
    float time_delta = controller.get_time_delta();
    for (const auto& bullet : bullets) {
        auto first_point = bullet->get_position();
        auto last_point = bullet->get_position_after_move(time_delta);
        bool is_moved_through_portal = false;
        for (const auto& portal : portals) {
            if (portal->crossed(first_point, last_point)) {
                Camera custom_camera;
                custom_camera.set_view_matrix(
                    glm::lookAt(first_point, last_point,
                                {0, 1, 0} /*random vector*/),
                    true);
                custom_camera =
                    get_portal_destination_camera(custom_camera, *portal);
                custom_camera.set_view_matrix(custom_camera.get_view_matrix(),
                                              true);
                bullet->set_position_by_camera(custom_camera);
                is_moved_through_portal = true;
                break;
            }
        }
        bullet->move(time_delta);
        if (is_moved_through_portal) {
            continue;
        }
        if (!bullet->is_visible()) {
            continue;
        }

        auto create_portal = [this, &bullet, &first_point,
                              &last_point](const auto& plane) {
            player_portals.replace_portal(
                glm::translate(glm::mat4(1.0), first_point),
                plane->get_rotation_matrix() * glm::rotate(glm::mat4(1.0),
                                                           (float)-M_PI_2,
                                                           glm::vec3{1, 0, 0}));
            bullet->set_unvisible();

            if (glm::dot(player_portals.get_first_portal_normal(),
                         last_point - first_point) > 0) {
                player_portals.move_first_portal(
                    glm::translate(glm::mat4(1.0), first_point),
                    plane->get_rotation_matrix() *
                        glm::rotate(glm::mat4(1.0), (float)M_PI_2,
                                    glm::vec3{1, 0, 0}));
            }
        };

        /*usual planes case*/ {
            bool is_first = true;
            for (const auto& plane_shared_ptr : primitives[PrimId<Plane>::id]) {
                if (auto plane =
                        dynamic_cast<const Plane*>(plane_shared_ptr.get());
                    plane) {
                    if (!is_first && plane->is_visible() &&
                        plane->crossed(first_point, last_point)) {
                        create_portal(plane);
                    }
                }
                is_first = false;
            }
        }

        /*planes from cubes case*/ {
            bool is_first = true;
            for (const auto& cube : primitives[PrimId<Cube>::id]) {
                if (!is_first) {
                    for (const auto& plane :
                         dynamic_cast<const Cube*>(cube.get())->get_planes()) {
                        if (plane->is_visible() &&
                            plane->crossed(first_point, last_point)) {
                            create_portal(plane);
                        }
                    }
                }
                is_first = false;
            }
        }
    }

    glm::vec3 first_point = controller.get_position();
    glm::vec3 last_point = controller.get_position_after_move();
    for (const std::shared_ptr<Portal>& portal : portals) {
        if (portal->crossed(first_point, last_point)) {
            Camera custom_camera =
                get_portal_destination_camera(camera, *portal);
            camera.set_view_matrix(custom_camera.get_view_matrix(), true);
        }
    }
    controller.cursor_position_callback();
    controller.key_callback();
    camera.update(controller.delta_time());
    controller.update_time();
}

void Scene::render_scene(const Camera& Cam, int recursion_level,
                         std::function<bool(const Entity*)> is_visible) const {



    auto draw_portals = [&](const Camera& cam) {
        for (auto& portal : portals) {
            portal->draw1(cam);
        }
    };

    auto draw_non_portals = [&](const Camera& cam) {
        glUniform4f(lighted_shader->get_uniform_id("color"), 0, 0, 0, 1);
        for (const auto& i : models) {
            for (unsigned j = 1; j < i.second.size(); ++j) {
                if (is_visible(i.second[j].get())) {
                    i.second[0]->move_to(i.second[j]);
                    i.second[0]->draw(cam, lights);
                }
            }
        }
        for (const auto& prim : primitives) {
            for (std::size_t i = 1; i < prim.size(); ++i) {
                if (is_visible(prim[i].get()) && prim[i]->is_visible()) {
                    prim[0]->move_to(prim[i]);
                    prim[0]->set_color(prim[i]->get_color());
                    prim[0]->draw(cam, lights);
                }
            }
        }
    };

    for (auto& portal : portals) {
        /*
         * Based on Thomas Rinsma’s blog post on rendering recursive portals:
         * https://th0mas.nl/2013/05/19/rendering-recursive-portals-with-opengl/
         */

        if (portal->is_support()) continue;
        // Calculate view matrix as if the player was already teleported
        Camera destination_camera = Cam;
        destination_camera =
            get_portal_destination_camera(destination_camera, *portal);

        // Disable color and depth drawing
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);

        // Disable depth test
        glDisable(GL_DEPTH_TEST);

        // Enable stencil test, to prevent drawing outside
        // region of current portal depth
        glEnable(GL_STENCIL_TEST);

        // Fail stencil test when inside of outer portal
        // (fail where we should be drawing the inner portal)
        glStencilFunc(GL_NOTEQUAL, recursion_level, 0xFF);

        // Increment stencil value on stencil fail
        // (on area of inner portal)
        glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);

        // Enable (writing into) all stencil bits
        glStencilMask(0xFF);

        // Draw portal into stencil buffer
        portal->draw1(Cam);

        // Base case, render inside of inner portal
        if (recursion_level == Settings::Portal::MAX_RECURSION_LEVEL) {
            // Enable color and depth drawing
            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
            glDepthMask(GL_TRUE);

            // Clear the depth buffer so we don't interfere with stuff
            // outside of this inner portal
            glClear(GL_DEPTH_BUFFER_BIT);

            // Enable the depth test
            // So the stuff we render here is rendered correctly
            glEnable(GL_DEPTH_TEST);

            // Enable stencil test
            // So we can limit drawing inside of the inner portal
            glEnable(GL_STENCIL_TEST);

            // Disable drawing into stencil buffer
            glStencilMask(0x00);

            // Draw only where stencil value == recursionLevel + 1
            // which is where we just drew the new portal
            glStencilFunc(GL_EQUAL, recursion_level + 1, 0xFF);

            // Draw scene objects with destView, limited to stencil buffer
            // use an edited projection matrix to set the near plane to the
            // portal plane
            draw_non_portals(destination_camera);
        } else {
            // Recursion case
            // Pass our new view matrix and the clipped projection matrix (see
            // above)
            auto check_if_visible = [&](const Entity* entity) {
                                if (auto plane = dynamic_cast<const
                                Plane*>(entity); plane) {
                                    return true;
                                }
                glm::vec3 normal = portal->get_destination()->get_normal();
                glm::vec3 entity_vector =
                    entity->get_position() -
                    portal->get_destination()->get_position();
                return glm::dot(normal, entity_vector) >= 0.0f;
            };
            render_scene(destination_camera, recursion_level + 1,
                         check_if_visible);
        }

        // Disable color and depth drawing
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);

        // Enable stencil test and stencil drawing
        glEnable(GL_STENCIL_TEST);
        glStencilMask(0xFF);

        // Fail stencil test when inside of our newly rendered
        // inner portal
        glStencilFunc(GL_NOTEQUAL, recursion_level + 1, 0xFF);

        // Decrement stencil value on stencil fail
        // This resets the incremented values to what they were before,
        // eventually ending up with a stencil buffer full of zero's again
        // after the last (outer) step.
        glStencilOp(GL_DECR, GL_KEEP, GL_KEEP);

        // Draw portal into stencil buffer
        portal->draw1(Cam);
    }

    // Disable the stencil test and stencil writing
    glDisable(GL_STENCIL_TEST);
    glStencilMask(0x00);

    // Disable color writing
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    // Enable the depth test, and depth writing.
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    // Make sure we always write the data into the buffer
    glDepthFunc(GL_ALWAYS);

    // Clear the depth buffer
    glClear(GL_DEPTH_BUFFER_BIT);

    // Draw portals into depth buffer
    draw_portals(Cam);

    // Reset the depth function to the default
    glDepthFunc(GL_LESS);

    // Enable stencil test and disable writing to stencil buffer
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0x00);

    // Draw at stencil >= recursionlevel
    // which is at the current level or higher (more to the inside)
    // This basically prevents drawing on the outside of this level.
    glStencilFunc(GL_LEQUAL, recursion_level, 0xFF);

    // Enable color and depth drawing again
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    // And enable the depth test
    glEnable(GL_DEPTH_TEST);

    // Draw scene objects normally, only at recursionLevel

    draw_non_portals(Cam);
    draw_portals(Cam);
}

void PairingPortals::replace_portal(glm::mat4 translation_matrix,
                                    glm::mat4 rotation_matrix) {
    std::swap(first, second);
    first->set_translation_matrix(translation_matrix);
    first->set_rotation_matrix(rotation_matrix);
}
void PairingPortals::set_portals(std::shared_ptr<Portal> first_,
                                 std::shared_ptr<Portal> second_) {
    first = first_;
    second = second_;
    first->set_destination(second.get());
    second->set_destination(first.get());
}
glm::vec3 PairingPortals::get_first_portal_normal() const {
    return first->get_normal();
}
void PairingPortals::move_first_portal(glm::mat4 translation_matrix,
                                       glm::mat4 rotation_matrix) {
    first->set_translation_matrix(translation_matrix);
    first->set_rotation_matrix(rotation_matrix);
}
