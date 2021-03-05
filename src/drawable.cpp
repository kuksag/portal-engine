#include "drawable.h"

#include <memory>
#include <utility>

Drawable::Drawable(std::shared_ptr<ShaderProgram> shader)
    : shader(std::move(shader)) {}