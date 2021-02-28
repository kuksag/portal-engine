#ifndef PORTAL_ENGINE_DRAWABLE_H
#define PORTAL_ENGINE_DRAWABLE_H

#include <vector>
#include <memory>

#include "entity.h"
#include "mesh.h"
#include "camera.h"

class Drawable : public Entity {
protected:

	std::shared_ptr<ShaderProgram> shader;

public:

	Drawable(std::shared_ptr<ShaderProgram> shader);

    virtual void draw(const Camera& camera) const = 0;

    virtual ~Drawable() = default;
};

#endif    // PORTAL_ENGINE_DRAWABLE_H
