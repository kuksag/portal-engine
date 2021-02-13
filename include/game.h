#ifndef PORTAL_ENGINE_GAME_H
#define PORTAL_ENGINE_GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "camera.h"
#include "controls.h"

struct Controller;

struct Game {
private:
    Camera *camera;
    Controller *controller;
    GLFWwindow *window;

    // TODO: add `RenderEngine` object, that actually draws graphics

    void window_initialise();

public:
    Game() : camera(new Camera()), controller(new Controller(this)), window() {
        window_initialise();

        // TODO:
        //  graphics_initialise(); <- step, where we load all resources (shaders,
        //  textures etc.)

        // TODO:
        //  world_initialise(); <- step, where we place all the objects
    }

    void run();

    friend struct Controller;
};

#endif    // PORTAL_ENGINE_GAME_H
