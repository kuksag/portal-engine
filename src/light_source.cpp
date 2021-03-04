#include "light_source.h"

glm::vec3 LightSource::get_color() const {
    return color;
}

glm::vec3 LightSource::get_position() const {
    return glm::mat3(get_model_matrix()) * position;
}

LightSource::LightSource(glm::vec3 position_, glm::vec3 color_): position(position_),
                                                            color(color_) {
}

