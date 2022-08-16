
#ifndef HEADER_SOLITAIRE
#define HEADER_SOLITAIRE

#include <vector>
#include <time.h>
#include "deck.h"
#include "stack.h"
#include "completed.h"
#include "utils.h"

class Solitaire 
{
public:
    Solitaire();
    void draw();
    void mouseDown(int mouseX, int mouseY);
    void mouseUp(int mouseX, int mouseY);
    void mouseDrag(int mouseX, int mouseY);
    void keyDown(SDL_Keysym keysym);
    void keyUp(SDL_Keysym keysym);
    Deck* getDeck();
    Stack* getStacks();
    Completed* getCompleted();
private:
    void reset();
    bool winner();

    Deck deck;
    Stack stacks[NUM_STACKS];
    Completed completed[4];
    MovingStack movingStack;
};

#endif // HEADER_SOLITAIRE