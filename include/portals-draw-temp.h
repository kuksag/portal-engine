#ifndef PORTAL_ENGINE_PORTALS_DRAW_TEMP_H
#define PORTAL_ENGINE_PORTALS_DRAW_TEMP_H

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/quaternion.hpp>

#include "SceneObjects/portal.h"
#include "camera.h"
#include "shader.h"

void draw_non_portals(
    std::vector<std::pair<Drawable *, ShaderProgram *>> &elements,
    glm::mat4 projection_view);

void magic(std::vector<std::pair<Drawable *, ShaderProgram *>> &elements,
           ShaderProgram &shader, Camera &camera, Portal &portal);
glm::mat4 clipped_projection_matrix(Portal &portal,
                                    glm::mat4 const &view_matrix,
                                    glm::mat4 const &projection_matrix);

#endif    // PORTAL_ENGINE_PORTALS_DRAW_TEMP_H
