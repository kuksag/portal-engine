#include "drawable.h"
#include <memory>

Drawable::Drawable(std::shared_ptr<ShaderProgram> shader) : shader(shader) {}