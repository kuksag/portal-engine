#include "portal_gun.h"
#include <utility>
#include "scene.h"
#include <iostream>

PortalGun::PortalGun(Scene &scene, Camera &camera)
 : scene(scene),
   camera(camera){
}

void PortalGun::launch_bullet() const {
    scene.add_bullet(camera.get_position(), camera.get_forward_direction());
}

Bullet::Bullet(glm::vec3 start_point, glm::vec3 direction, std::shared_ptr<Primitive> ball_) :
 direction(glm::normalize(direction)),
 ball(std::move(ball_)){
    ball->scale(glm::vec3(0.03f));
    translate(start_point);
    ball->translate(start_point);
}

void Bullet::move(float time_delta) {
    static const float speed = 7.0f;
    translate(speed * time_delta * direction);
    ball->translate(speed * time_delta * direction);
}
