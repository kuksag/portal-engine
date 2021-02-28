#include "light_source.h"

glm::vec3 LightSource::get_color() const {
    return color;
}

glm::vec3 LightSource::get_pos() const {
    return glm::mat3(get_model_matrix()) * pos;
}

LightSource::LightSource(glm::vec3 pos_, glm::vec3 color_): pos(pos_),
                                                            color(color_) {
}

