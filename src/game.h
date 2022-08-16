
#ifndef HEADER_GAME
#define HEADER_GAME

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "solitaire.h"
#include "solver.h"

const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

class Game 
{
public:
    Game();
    ~Game();
    bool init();
    bool mainLoop();
    SDL_Renderer* getRenderer();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Solitaire* solitaire;
    Solver solver;
};

#endif // HEADER_GAME