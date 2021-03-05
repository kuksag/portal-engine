#ifndef PORTAL_ENGINE_DRAWABLE_H
#define PORTAL_ENGINE_DRAWABLE_H

#include <memory>
#include <vector>

#include "camera.h"
#include "entity.h"
#include "light_source.h"
#include "mesh.h"

class Drawable : public Entity { //TODO: don't let user create this
protected:

    std::shared_ptr<ShaderProgram> shader;

public:
    virtual void draw(const Camera& camera, const std::vector<LightSource>& light_sources) const = 0;
    explicit Drawable(std::shared_ptr<ShaderProgram> shader);

    ~Drawable() override = default;
};

#endif    // PORTAL_ENGINE_DRAWABLE_H
