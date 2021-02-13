#ifndef PORTAL_ENGINE_CONTROLS_H
#define PORTAL_ENGINE_CONTROLS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct Game;

struct Controller {
private:
    // TODO: use unique_ptr?
    Game *game;
    float last_time_point;

public:
    explicit Controller(Game *game_)
        : game(game_), last_time_point() {
        update_time();
    }

    void scroll_callback(double y_delta);
    void cursor_position_callback(double x_delta, double y_delta);
    void key_callback(int key, int action);

    void update_time();

    // TODO:
    //    void mouse_button_callback(int button, int action, int mods);
    //    void window_focus_callback(int focused);
};

void scroll_callback(GLFWwindow *window, double x_delta, double y_delta);
void cursor_position_callback(GLFWwindow *window, double x_delta,
                                     double y_delta);
void key_callback(GLFWwindow *window, int key, int scancode, int action,
                         int mods);

// TODO:
//  static void mouse_button_callback(GLFWwindow *window, int button, int
//  action,
//                                  int mods);
//  static void window_focus_callback(GLFWwindow *window, int focused);

#endif    // PORTAL_ENGINE_CONTROLS_H
