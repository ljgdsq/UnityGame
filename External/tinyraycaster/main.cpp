#include <iostream>
#define SDL_MAIN_HANDLED
#include "SDL.h"

int main() {

    if (SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
