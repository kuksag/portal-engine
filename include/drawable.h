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
    const std::vector<LightSource> *light_sources{}; //TODO: vector<LightSources*>, чтобы не было рассинхрона

public:
    virtual void draw(const Camera& camera) const = 0;
    explicit Drawable(std::shared_ptr<ShaderProgram> shader);

    void set_light_sources(const std::vector<LightSource> *data);

    ~Drawable() override = default;
};

#endif    // PORTAL_ENGINE_DRAWABLE_H
