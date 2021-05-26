#ifndef PORTAL_ENGINE_PORTAL_GUN_H
#define PORTAL_ENGINE_PORTAL_GUN_H
#include "entity.h"
#include "primitives.h"


class Scene;
struct Camera;

struct PortalGun : Entity {
private:
    Scene &scene;
    Camera &camera;
public:
    PortalGun(Scene &scene, Camera &camera);
    void launch_bullet() const;
};

struct Bullet : Entity {
private:
    inline static const float Speed = 10.0f;
    glm::vec3 start_point;
    glm::vec3 direction;
    std::shared_ptr<Primitive> ball;
public:
    inline static const glm::vec3 Bullet_Color = glm::vec3(0.5f, 0.3f, 0.4f);
    Bullet(glm::vec3 start_point_, glm::vec3 direction_, std::shared_ptr<Primitive> ball_);
    void move(float time_delta) override;
    glm::vec3 get_position_after_move(float time_delta);
    void translate(const glm::vec3 &data) override;
    void set_position_by_camera(const Camera &camera);
    void set_unvisible();
};

#endif    // PORTAL_ENGINE_PORTAL_GUN_H
