#include "portal_gun.h"
#include <utility>
#include "scene.h"
#include <iostream>
#include "camera.h"

PortalGun::PortalGun(Scene &scene, Camera &camera)
 : scene(scene),
   camera(camera){
}

void PortalGun::launch_bullet() const {
    scene.add_bullet(camera.get_position(), camera.get_forward_direction());
}

Bullet::Bullet(glm::vec3 start_point_, glm::vec3 direction_, std::shared_ptr<Primitive> ball_) :
 start_point(std::move(start_point_)),
 direction(glm::normalize(direction_)),
 ball(std::move(ball_)){
    ball->scale(glm::vec3(0.03f));
    translate(start_point);
}

void Bullet::move(float time_delta) {
    static const float speed = 7.0f;
    translate(speed * time_delta * direction);
}

void Bullet::translate(const glm::vec3 &data) {
    Entity::translate(data);
    ball->translate(data);
}

void Bullet::change_direction(const glm::vec3 &direction1,
                              const glm::vec3 &direction2) {

}
