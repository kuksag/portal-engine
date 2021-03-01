#include "portals-draw-temp.h"

void magic(std::vector<std::pair<Drawable*, ShaderProgram*>>& elements,
           ShaderProgram& shader, Camera& camera, Portal& portal) {
    int recursionLevel = 0;

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
    glStencilFunc(GL_NOTEQUAL, recursionLevel, 0xFF);

    // Increment stencil value on stencil fail
    // (on area of inner portal)
    glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);

    // Enable (writing into) all stencil bits
    glStencilMask(0xFF);

    // Draw portal into stencil buffer
    portal.draw(shader,
                camera.get_projection_matrix() * camera.get_view_matrix());

    // Calculate view matrix as if the player was already teleported
    glm::mat4 destination_view =
        camera.get_view_matrix() * portal.get_model_matrix() *
        glm::rotate(glm::mat4(1.0f), 180.0f,
                    glm::vec3(0.0f, 1.0f, 0.0f) *
                        portal.get_quaternion_rotation_matrix()) *
        glm::inverse(portal.get_destination()->get_model_matrix());

    // Base case, render inside of inner portal
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
    glStencilFunc(GL_EQUAL, recursionLevel + 1, 0xFF);

    // Draw scene objects with destView, limited to stencil buffer
    // use an edited projection matrix to set the near plane to the portal
    // plane
    draw_non_portals(elements,
                     clipped_projection_matrix(portal, destination_view,
                                               camera.get_projection_matrix()) *
                         destination_view);
    // drawNonPortals(destView, projMat);

    // Disable color and depth drawing
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);

    // Enable stencil test and stencil drawing
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xFF);

    // Fail stencil test when inside of our newly rendered
    // inner portal
    glStencilFunc(GL_NOTEQUAL, recursionLevel + 1, 0xFF);

    // Decrement stencil value on stencil fail
    // This resets the incremented values to what they were before,
    // eventually ending up with a stencil buffer full of zero's again
    // after the last (outer) step.
    glStencilOp(GL_DECR, GL_KEEP, GL_KEEP);

    // Draw portal into stencil buffer
    portal.draw(shader,
                camera.get_projection_matrix() * camera.get_view_matrix());

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
    portal.draw(shader,
                camera.get_projection_matrix() * camera.get_view_matrix());

    // Reset the depth function to the default
    glDepthFunc(GL_LESS);

    // Enable stencil test and disable writing to stencil buffer
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0x00);

    // Draw at stencil >= recursionlevel
    // which is at the current level or higher (more to the inside)
    // This basically prevents drawing on the outside of this level.
    glStencilFunc(GL_LEQUAL, recursionLevel, 0xFF);

    // Enable color and depth drawing again
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);

    // And enable the depth test
    glEnable(GL_DEPTH_TEST);

    // Draw scene objects normally, only at recursionLevel
    draw_non_portals(elements,
                     camera.get_projection_matrix() * camera.get_view_matrix());
}

glm::mat4 clipped_projection_matrix(Portal& portal,
                                    glm::mat4 const& view_matrix,
                                    glm::mat4 const& projection_matrix) {
    float dist = glm::length(portal.get_center());
    glm::vec4 clipPlane(
        portal.get_quaternion_rotation_matrix() * glm::vec3(0.0f, 0.0f, -1.0f),
        dist);
    clipPlane = glm::inverse(glm::transpose(view_matrix)) * clipPlane;

    if (clipPlane.w > 0.0f) return projection_matrix;

    glm::vec4 q =
        glm::inverse(projection_matrix) *
        glm::vec4(glm::sign(clipPlane.x), glm::sign(clipPlane.y), 1.0f, 1.0f);

    glm::vec4 c = clipPlane * (2.0f / (glm::dot(clipPlane, q)));

    glm::mat4 newProj = projection_matrix;
    // third row = clip plane - fourth row
    newProj = glm::row(newProj, 2, c - glm::row(newProj, 3));

    return newProj;
}
