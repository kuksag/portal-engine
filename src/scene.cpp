#include "scene.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "settings.h"

#include "window.h"
#include "portal.h"
#include "camera.h"
#include "controls.h"
#include <iostream>

Scene::Scene(Window& window, Camera& camera, Controller& controller) : window(window), camera(camera), controller(controller), primitives(PORTAL_ENGINE_PRIMITIVES_COUNT), lighted_shader(new ShaderProgram("shaders/light.vertex", "shaders/light.fragment")),
                                                                      portal_gun(*this, camera)  {
	glClearColor(bg_color.x, bg_color.y, bg_color.z, 0.0f);
	glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void Scene::draw() const {
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilMask(0xFF);
	glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// int k = 0;
	// for (const auto& prim : primitives) {
	// 	if (k++ == 2)
	// 		continue;
	// 	for (unsigned i=1; i < prim.size(); ++i) {
	// 		prim[0]->move_to(prim[i]);
	// 		prim[0]->set_color(prim[i]->get_color());
	// 		prim[0]->draw(camera, lights);
	// 	}
	// }

	// for (auto i : models) {
	// 	for (unsigned j=1; j < i.second.size(); ++j) {
	// 		i.second[0]->move_to(i.second[j]);
	// 		i.second[0]->draw(camera, lights);
	// 	}
	// }
        if (controller.is_enter_pressed()) {
            portal_gun.launch_bullet();
        }
        float time_delta = controller.get_time_delta();
        for (const auto &bullet : bullets) {
            auto first_point = bullet->get_position();
            bullet->move(time_delta);
            auto last_point = bullet->get_position();
            for (const auto &portal : portals) {
                if (portal->crossed(first_point, last_point)) {
                    Camera custom_camera;
                    custom_camera.set_view_matrix(glm::lookAt(last_point, last_point + last_point - first_point, {40, 28, 34} /*random vector*/));
                    custom_camera = get_portal_destination_camera(custom_camera, *portal);
                    bullet->set_position_by_camera(custom_camera);
                    break;
                }
            }
        }

    glm::vec3 first_point = controller.get_position();
    glm::vec3 last_point = controller.get_position_after_move();
    for (const std::shared_ptr<Portal> &portal : portals) {
        if (portal->crossed(first_point, last_point)) {
            camera = get_portal_destination_camera(camera, *portal);
        }
    }
    controller.cursor_position_callback();
    controller.key_callback();
    camera.update(controller.delta_time());
    controller.update_time();

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


    render_scene(camera, primitives, portals, 0);

	glfwSwapBuffers(window.glfw_window());
}

std::shared_ptr<Model> Scene::add_model(const std::string& path, const glm::vec3& position) {
	if (models[path].empty())
		models[path].push_back(std::make_shared<Model>(path, lighted_shader, true));
	models[path].push_back(std::make_shared<Model>(path, lighted_shader, false));
	return models[path].back();
}

std::shared_ptr<LightSource> Scene::add_light(const glm::vec3& position, const glm::vec3& color, float intensity, bool is_shadowed) {
	lights.push_back(std::make_shared<LightSource>(position, color, intensity, is_shadowed));
	return lights.back();
}

std::shared_ptr<Portal> Scene::add_portal(const glm::vec3& position) {
	portals.push_back(std::make_shared<Portal>(this, lighted_shader));
	portals.back()->translate(position);
	return portals.back();
}

std::shared_ptr<Bullet> Scene::add_bullet(const glm::vec3& start_point,
                                          const glm::vec3& direction) {
    auto ball = add_primitive<Sphere>({0, 0, 0}, Bullet::Bullet_Color);
    bullets.push_back(std::shared_ptr<Bullet>(new Bullet(start_point, direction, ball)));
    return bullets.back();
}

namespace {
void draw_non_portals(const Camera &camera,
                      const std::vector< std::vector<std::shared_ptr<Primitive>> > &primitives,
                      const std::vector<std::shared_ptr<Portal>> &portals,
                      const std::vector<std::shared_ptr<LightSource>>& lights) {
    for (const auto& prim : primitives) {
     for (unsigned i=1; i < prim.size(); ++i) {
         if (prim[i]->get_color() != glm::vec3(-1, -1, -1)) {
             prim[0]->move_to(prim[i]);
             prim[0]->set_color(prim[i]->get_color());
             prim[0]->draw(camera, lights);
         }
     }
    }
}

void draw_portals(const Camera &camera,
                      const std::vector<std::shared_ptr<Portal>> &portals) {
    for (auto &portal : portals) {
            portal->draw1(camera);
    }
}

}


void Scene::set_bg_color(const glm::vec3& color) {
	bg_color = color;
	glClearColor(bg_color.x, bg_color.y, bg_color.z, 0.0f);
}

void Scene::render_scene(const Camera &camera,
                  const std::vector< std::vector<std::shared_ptr<Primitive>> >& objects,
                  const std::vector<std::shared_ptr<Portal>> &portals,
                  int recursion_level = 0) const {
    for (auto &portal : portals) {
        // Calculate view matrix as if the player was already teleported
        Camera destination_camera = camera;
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
        portal->draw1(camera);

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
            draw_non_portals(destination_camera, objects, portals, lights);
        } else {
            // Recursion case
            // Pass our new view matrix and the clipped projection matrix (see
            // above)
            render_scene(destination_camera, objects, portals,
                         recursion_level + 1);
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
        portal->draw1(camera);
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
    draw_portals(camera, portals);

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

    draw_non_portals(camera, objects, portals, lights);
    draw_portals(camera, portals);
}