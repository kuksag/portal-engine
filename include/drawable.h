#ifndef PORTAL_ENGINE_DRAWABLE_H
#define PORTAL_ENGINE_DRAWABLE_H

#include "light_source.h"
#include <vector>
#include <memory>

#include "entity.h"
#include "mesh.h"
#include "camera.h"



class Drawable : public Entity {
protected:

	std::shared_ptr<ShaderProgram> shader;

public:

    virtual void draw(const Camera& camera, const std::vector<LightSource> &light_sources) const = 0;
    Drawable(std::shared_ptr<ShaderProgram> shader);

    virtual ~Drawable() = default;
};

#endif    // PORTAL_ENGINE_DRAWABLE_H
