#ifndef PORTAL_ENGINE_LIGHT_SOURCE_H
#define PORTAL_ENGINE_LIGHT_SOURCE_H

#include <glm/gtc/matrix_transform.hpp>
#include "camera.h"
#include "entity.h"
#include "shader.h"
#include <memory>
struct Drawable;

struct LightSource : public Entity {
private:
    static const int SHADOW_WIDTH = 3000, SHADOW_HEIGHT = 3000;
    glm::vec3 color;
    Camera camera;
    GLuint depth_map;
    GLuint depth_map_fbo;

    void init_depth_map();
public:
    LightSource(glm::vec3 position_, glm::vec3 color_);
    [[nodiscard]] glm::vec3 get_position() const;
    [[nodiscard]] glm::vec3 get_color() const;
    [[nodiscard]] const Camera &get_camera() const;
    [[nodiscard]] GLuint get_depth_map() const;
    void gen_depth_map(const std::vector<Drawable *> &drawables);
};

#endif    // PORTAL_ENGINE_LIGHT_SOURCE_H
