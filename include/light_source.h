#ifndef PORTAL_ENGINE_LIGHT_SOURCE_H
#define PORTAL_ENGINE_LIGHT_SOURCE_H

#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "camera.h"
#include "entity.h"
#include "shader.h"
struct Drawable;

struct LightSource : public Entity {
private:
    static const int SHADOW_WIDTH = 4000, SHADOW_HEIGHT = 4000;
    glm::vec3 color;
    Camera camera;
    GLuint depth_map;
    GLuint depth_map_fbo;
    float intensity_;
    bool is_shadowed;

    void init_depth_map();

public:
    LightSource(glm::vec3 position_, glm::vec3 color_, float intensity,
                bool is_shadowed_);
    [[nodiscard]] float intensity() const;
    [[nodiscard]] bool shadowed() const;
    [[nodiscard]] glm::vec3 get_position() const;
    [[nodiscard]] glm::vec3 get_color() const;
    [[nodiscard]] const Camera &get_camera() const;
    [[nodiscard]] GLuint get_depth_map() const;
    void start_depth_test();
    void finish_depth_test();
    void gen_depth_map(Drawable const *drawable);
};

#endif    // PORTAL_ENGINE_LIGHT_SOURCE_H
