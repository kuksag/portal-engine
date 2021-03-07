#include "light_source.h"

glm::vec3 LightSource::get_color() const {
    return color;
}

glm::vec3 LightSource::get_position() const {
    return glm::vec3(get_model_matrix() * glm::vec4(glm::vec3(), 1.0f));
}

LightSource::LightSource(glm::vec3 position_, glm::vec3 color_):
                                                            color(color_) {
    translate(position_);
    camera.set_view_matrix(glm::lookAt(position_,
                                       glm::vec3(0.0f, 0.0f, 0.0f) - position_,
                                       glm::vec3(0.0f, 1.0f, 0.0f)));
}

const Camera &LightSource::get_camera() const {
    return camera;
}

void LightSource::set_depth_map(GLuint id) {
    depth_map = id;
}

GLuint LightSource::get_depth_map() const {
    return  depth_map;
}
