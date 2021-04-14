#include "puzzle.h"

void JokersTrap::init_portals() {
    const float BOUND_MATCH = 0.9;

    for (std::size_t i = 0; i < 2; i++) {
        for (std::size_t j = 0; j < EDGE_NUMBER; j++)
            portals[i][j] = new Portal;
    }

    static const float x_offset[] = {0, 0, 1, -1, 0, 0};
    static const float y_offset[] = {0, 0, 0, 0, 1, -1};
    static const float z_offset[] = {1, -1, 0, 0, 0, 0};

    for (std::size_t j = 0; j < EDGE_NUMBER; j++) {
        portals[0][j]->translate({x_offset[j], y_offset[j], z_offset[j]});
        portals[0][j]->scale({glm::vec3(BOUND_MATCH)});
        // This scale is needed in order to have a perfect cube
    }

    portals[0][0]->rotate(M_PI, {0.0, 1.0, 0.0});
    portals[0][2]->rotate(-M_PI_2, {0.0, 1.0, 0.0});
    portals[0][3]->rotate(M_PI_2, {0.0, 1.0, 0.0});
    portals[0][4]->rotate(M_PI_2, {1.0, 0.0, 0.0});
    portals[0][5]->rotate(-M_PI_2, {1.0, 0.0, 0.0});

    for (std::size_t i = 0; i < EDGE_NUMBER; i++)
        portals[1][i]->translate(glm::vec3((i + 1) * MOVE_DISTANCE));


    for (std::size_t i = 0; i < EDGE_NUMBER; i++)
        portals[0][i]->set_destination(portals[1][i]);
    portals[0][2]->set_destination(portals[1][3]);
    portals[1][3]->set_destination(portals[0][2]);
    portals[0][3]->set_destination(portals[1][2]);
    portals[1][2]->set_destination(portals[0][3]);
}

void JokersTrap::init_objects() {
    const float SIZE = 1.0;
    const float OFFSET = 1.5;

    objects[0] = new Sphere;
    objects[1] = new Cube;
    objects[2] = new Torus;
    objects[3] = new Cylinder;
    objects[4] = new Cone;
    objects[5] = new Cube;

    for (std::size_t i = 0; i < EDGE_NUMBER; i++) {
        objects[i]->scale(glm::vec3(SIZE));
        objects[i]->translate(glm::vec3((i + 1) * MOVE_DISTANCE));
        objects[i]->translate({0.0, 0.0, -OFFSET});
    }
    objects[1]->scale(glm::vec3(0.7));
}

JokersTrap::JokersTrap()
    : portals(2, std::vector<Portal *>(EDGE_NUMBER)), objects(EDGE_NUMBER) {
    init_portals();
    init_objects();
}
