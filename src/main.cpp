#include <iostream>
#include <string>

#include "game.h"
int main() {
    try {
        Game game;
        game.run();
    } catch (std::string &message) {
        std::cerr << "Something went wrong:\n" << message;
        return -1;
    }
    return 0;
}
