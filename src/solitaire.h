
#ifndef HEADER_SOLITAIRE
#define HEADER_SOLITAIRE

#include <vector>
#include <time.h>
#include "stack.h"

const int NUM_STACKS = 7;

class Solitaire 
{
public:
    Solitaire();
    void drawBoard();
    void mouseDown(int mouseX, int mouseY);
    void mouseUp(int mouseX, int mouseY);
    void mouseDrag(int mouseX, int mouseY);
    void keyDown(SDL_Keysym keysym);
    void keyUp(SDL_Keysym keysym);
private:
    void resetBoard();
    void shuffle(std::vector<int>& vec);
    bool cardCanBePlacedOnCompleted(int card, int stack);
    bool isMouseInsideRect(int mouseX, int mouseY, SDL_Rect* rect);

    SDL_Rect backDeckRect;
    SDL_Rect frontDeckRect;
    SDL_Rect completedRects[4];

    bool spacePressed;
    int deckIndex;
    MovingStack movingStack;
    int cardsTopCompleted[4];
    std::vector<int> cardsDeck;
    Stack stacks[NUM_STACKS];
};

#endif // HEADER_SOLITAIRE