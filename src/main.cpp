#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SceneObjects/cube.h>
#include <SceneObjects/portal.h>
#include <SceneObjects/pyramid.h>
#include <SceneObjects/triangle.h>

#include <iostream>

#include "camera.h"
#include "controls.h"
#include "portals-draw-temp.h"
#include "settings.h"

void draw_non_portals(
    std::vector<std::pair<Drawable *, ShaderProgram *>> &elements,
    glm::mat4 projection_view) {
    for (auto &element : elements) {
        element.first->draw(*element.second, projection_view);
    }
}

int main() {
    GLFWwindow *window = nullptr;
    Camera camera;
    Controller controller(&camera, window);
    // -------------------------------------------------------------------------
    ShaderProgram triangle_shader("Triangle/triangle.vertex",
                                  "Triangle/triangle.fragment");
    ShaderProgram light_shader("shaders/light.vertex",
                               "shaders/light.fragment");
    ShaderProgram pyramid_shader("Pyramid/pyramid.vertex",
                                 "Pyramid/pyramid.fragment");
    ShaderProgram portal_shader("Portal/portal.vertex",
                                "Portal/portal.fragment");
    ShaderProgram beacon_shader("Portal/portal.vertex",
                                "Portal/portal.fragment");
    // -------------------------------------------------------------------------
    Triangle textured_triangle;
    Cube cube_of_shades;
    Pyramid rainbow_pyramid;
    Portal portal_a, portal_b;
    // -------------------------------------------------------------------------
    portal_a.translate({-2.5, 1.0, 2.0});
    portal_a.rotate(-M_PI_2, {0.0, 1.0, 0.0});
    float SIZE = 3;
    portal_a.scale({SIZE, SIZE, SIZE});
    // ---------------------------------
    portal_b.translate({0, 2.0, 6.0});
    portal_b.scale({SIZE, SIZE, SIZE});
    // ---------------------------------
    portal_a.set_destination(&portal_b);
    portal_b.set_destination(&portal_a);
    // -------------------------------------------------------------------------
    glEnable(GL_DEPTH_TEST);
    // -------------------------------------------------------------------------
    std::vector<std::pair<Drawable *, ShaderProgram *>> elements;
    elements.emplace_back(&textured_triangle, &triangle_shader);
    elements.emplace_back(&rainbow_pyramid, &pyramid_shader);
    elements.emplace_back(&cube_of_shades, &light_shader);
    // -------------------------------------------------------------------------
    std::vector<Portal*> portals;
    portals.push_back(&portal_a);
    portals.push_back(&portal_b);
    // -------------------------------------------------------------------------
    do {
        controller.cursor_position_callback();
        controller.key_callback();
        controller.update_time();

        glClearColor(0.3f, 0.3f, 0.6f, 0.0f);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);
        glStencilMask(0xFF);
        glClear(GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        // ---------------------------------------------------------------------
        //         calculation for rainbow-pyramid
        float radius = 3.0f;
        glm::vec3 pivot = {3.0f, 3.0f, 3.0f};
        float time_value = glfwGetTime();
        auto color_per_frame = glm::vec3(sin(time_value), 0.2, cos(time_value));
        auto rotation_per_frame = glm::vec3(6 * M_PI, M_PI, M_PI_2);
        auto translate_per_frame =
            pivot + glm::vec3(cos(time_value / 2) * radius, sin(time_value / 2),
                              sin(time_value / 2) * radius);

        // ---------------------------------------------------------------------
        // pyramid draw
        Pyramid::supply_shader(pyramid_shader, color_per_frame);
        rainbow_pyramid.set_rotation_matrix(
            glm::rotate(glm::mat4(1.0f), time_value, rotation_per_frame));
        rainbow_pyramid.set_translation_matrix(
            glm::translate(glm::mat4(1.0f), translate_per_frame));
        //
        // ---------------------------------------------------------------------
        // "cube of shade" draw
        cube_of_shades.set_camera_pos(light_shader, camera.get_position());
        cube_of_shades.set_translate(light_shader, {3.0f, 3.0f, 3.0f});
        cube_of_shades.set_light_color(light_shader, color_per_frame);
        cube_of_shades.set_light_position(light_shader,
                                          rainbow_pyramid.get_center());
        // ---------------------------------------------------------------------
//                portal_a.draw(portal_shader, camera.get_projection_matrix() *
//                                                 camera.get_view_matrix());
//                portal_a.draw_shape(portal_shader, camera.get_projection_matrix() *
//                                                   camera.get_view_matrix());
        // ---------------------------------------------------------------------
//                portal_b.draw(portal_shader, camera.get_projection_matrix() *
//                                                 camera.get_view_matrix());
//                portal_b.draw_shape(portal_shader, camera.get_projection_matrix() *
//                                                   camera.get_view_matrix());
        // ---------------------------------------------------------------------
//                draw_non_portals(elements, camera.get_projection_matrix() *
//                                               camera.get_view_matrix());
        // ---------------------------------------------------------------------
        magic(elements, portal_shader, camera, portals);
        // ---------------------------------------------------------------------

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glfwTerminate();
}
