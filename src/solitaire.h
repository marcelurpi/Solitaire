
#ifndef HEADER_SOLITAIRE
#define HEADER_SOLITAIRE

#include <vector>
#include <time.h>
#include "stack.h"
#include "deck.h"

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
    bool cardCanBePlacedOnCompleted(int card, int stack);
    bool isMouseInsideRect(int mouseX, int mouseY, SDL_Rect* rect);

    SDL_Rect completedRects[4];

    int cardsTopCompleted[4];

    Deck deck;
    Stack stacks[NUM_STACKS];
    MovingStack movingStack;
};

#endif // HEADER_SOLITAIRE