#include "drawable.h"

#include <memory>
#include <utility>

Drawable::Drawable(std::shared_ptr<ShaderProgram> shader)
    : shader(std::move(shader)) {}

void Drawable::set_light_sources(const std::vector<LightSource> *data) {
    light_sources = data;
}
