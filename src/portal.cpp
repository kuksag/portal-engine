#include "portal.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

#include "settings.h"

namespace {
void draw_non_portals(const Camera &camera,
                      const std::vector<Drawable *> &objects,
                      const std::vector<Portal *> &portals) {
    for (auto &object : objects) object->draw(camera);
//    draw_portals(camera, portals, true);
}
}    // namespace

void draw_portals(const Camera &camera, const std::vector<Portal *> &portals,
                  bool draw_bounds = false) {
    for (auto &portal : portals) {
        if (draw_bounds) {
            portal->draw_bounds(camera);
        } else {
            portal->draw(camera);
        }
    }
}

Portal::Portal()
    : Drawable(std::make_shared<ShaderProgram>("shaders/color.vertex",
                                               "shaders/color.fragment")),
      destination(this) {
    // -------------------------------------------------------------------------
    shape.scale({1.0, 1.0, 0.1});
    // -------------------------------------------------------------------------
    beacon.scale({0.2, 0.2, 0.2});
    beacon.set_color({0.0, 1.0, 0.0});
    beacon.rotate(M_PI_2, {0.0, 1.0, 0.0});
    beacon.rotate(-M_PI_2, {0.0, 0.0, 1.0});
    beacon.translate({0.0, 0.0, -0.5});
    // -------------------------------------------------------------------------
    bounds.resize(4);
    for (auto &bound : bounds) {
        bound.scale({1.2, 0.1, 0.1});
        bound.set_color({0.0, 0.0, 1.0});
    }
    bounds[0].translate({0.0, -1.1, 0.0});
    bounds[1].translate({0.0, 1.1, 0.0});

    bounds[2].translate({-1.1, 0.0, 0.0});
    bounds[2].rotate(M_PI_2, {0.0, 0.0, 1.0});

    bounds[3].translate({1.1, 0.0, 0.0});
    bounds[3].rotate(M_PI_2, {0.0, 0.0, 1.0});
    // -------------------------------------------------------------------------
}

void Portal::draw(const Camera &camera) const {
    auto custom_camera = camera;
    custom_camera.set_view_matrix(camera.get_view_matrix() *
                                  get_model_matrix());
    shape.draw(custom_camera);
}

void Portal::draw_bounds(const Camera &camera) const {
    auto custom_camera = camera;
    custom_camera.set_view_matrix(camera.get_view_matrix() *
                                  get_model_matrix());
    //    beacon.draw(custom_camera);
    for (auto &bound : bounds) bound.draw(custom_camera);
}

void Portal::set_destination(Portal *other) { destination = other; }

glm::vec3 Portal::get_center() const {
    auto result = get_model_matrix() * glm::vec4(glm::vec3(0.0f), 1.0f);
    return glm::vec3(result[0], result[1], result[2]);
}

Camera get_portal_destination_camera(const Camera &camera,
                                     const Portal &portal) {
    auto result = camera;
    result.set_view_matrix(
        // 3. Transformation from source portal to the camera -- it's the
        //    first portal's ModelView matrix:
        camera.get_view_matrix() * portal.get_model_matrix() *
        // 2. Object is front-facing, the camera is facing the other way:
        glm::rotate(glm::mat4(1.0), static_cast<float>(M_PI),
                    glm::quat_cast(portal.get_rotation_matrix()) *
                        glm::vec3(0.0, 1.0, 0.0)) *
        // 1. Go the destination portal; using inverse, because camera
        //    transformations are reversed compared to object
        //    transformations:
        glm::inverse(portal.destination->get_model_matrix()));
    return result;
}

void render_scene(const Camera &camera, const std::vector<Drawable *> &objects,
                  const std::vector<Portal *> &portals,
                  int recursion_level = 0) {
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
        portal->draw(camera);

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
            draw_non_portals(destination_camera, objects, portals);
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
        portal->draw(camera);
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
//    draw_portals(camera, portals, true);

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
    draw_non_portals(camera, objects, portals);
    draw_portals(camera, portals, true);

}
