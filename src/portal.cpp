#include "portal.h"

Portal::Portal()
    : Drawable(std::make_shared<ShaderProgram>("shaders/color.vertex",
                                               "shaders/color.fragment")),
      destination(this) {}

void Portal::draw(const Camera& camera) const {

}
