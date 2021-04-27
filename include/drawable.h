#ifndef PORTAL_ENGINE_DRAWABLE_H
#define PORTAL_ENGINE_DRAWABLE_H

#include <memory>
#include <vector>

#include "camera.h"
#include "entity.h"
#include "light_source.h"
#include "mesh.h"

class Drawable : public Entity {
protected:
    std::shared_ptr<ShaderProgram> shader;

public:
    virtual void draw(const Camera& camera, const std::vector< std::shared_ptr<LightSource> >& ls) const = 0;
    virtual void depth_test_draw(
        const Camera& camera,
        std::shared_ptr<ShaderProgram> depth_shader) const = 0;
    explicit Drawable(std::shared_ptr<ShaderProgram> shader);

    ~Drawable() override = default;
};
#endif    // PORTAL_ENGINE_DRAWABLE_H
