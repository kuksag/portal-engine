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
}

