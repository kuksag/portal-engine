#ifndef PORTAL_ENGINE_CAMERA_H
#define PORTAL_ENGINE_CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "settings.h"
using namespace Settings::Camera;

struct Camera {
private:
    glm::vec3 position;
    float horizontal_angle;
    float vertical_angle;

    float move_speed;
    float mouse_sensitivity;
    float scroll_sensitivity;
    float fov;
    float ratio;
    float display_range_near;
    float display_range_far;

    glm::vec3 speed = {0, 0, 0};
    static constexpr glm::vec3 acceleration = {0, -9, 0};
    static constexpr glm::vec3 jump_speed = {0, 4, 0};

    bool is_on_floor = false;

    bool custom_state_view_matrix{false};
    glm::mat4 custom_view_matrix{};

public:
    explicit Camera(glm::vec3 position_ = glm::vec3(POS_X, POS_Y, POS_Z),
                    float horizontal_angle_ = HORIZONTAL_ANGLE,
                    float vertical_angle_ = VERTICAL_ANGLE)
        : position(position_),
          horizontal_angle(horizontal_angle_),
          vertical_angle(vertical_angle_),
          move_speed(MOVE_SPEED),
          mouse_sensitivity(MOUSE_SENSITIVITY),
          scroll_sensitivity(SCROLL_SENSITIVITY),
          fov(FOV),
          ratio(RATIO),
          display_range_near(DISPLAY_RANGE_NEAR),
          display_range_far(DISPLAY_RANGE_FAR) {}

    [[nodiscard]] glm::vec3 get_position() const;
    [[nodiscard]] glm::vec3 get_forward_direction() const;
    [[nodiscard]] glm::vec3 get_right_direction() const;
    [[nodiscard]] glm::vec3 get_up_direction() const;

    [[nodiscard]] glm::mat4 get_projection_matrix() const;
    [[nodiscard]] glm::mat4 get_view_matrix() const;

    void update(float dt);
    void jump();

    void move_forward(float time_delta);
    void move_backward(float time_delta);
    void move_right(float time_delta);
    void move_left(float time_delta);
    void move_up(float time_delta);
    void move_down(float time_delta);

    void process_mouse_move(float x_delta, float y_delta, float time_delta,
                            bool constraint = true);
    void process_mouse_scroll(float y_delta);

    // -------------------------------------------------------------------------
    // This is used for easy transfer of view\projection matrix, when calling
    // Drawable::draw(); also because we might apply additional model-matrix.
    //
    // When its called, all following calls of any function, except
    // get_projection_matrix or get_view_matrix,
    // will do or return nonsense (but not UB)
    void set_view_matrix(glm::mat4 data, bool recalculate = false);
    // -------------------------------------------------------------------------
};

#endif    // PORTAL_ENGINE_CAMERA_H
