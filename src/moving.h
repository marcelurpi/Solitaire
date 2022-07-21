
#ifndef HEADER_MOVING
#define HEADER_MOVING

#include "textureManager.h"
#include "stack.h"
#include <vector>

struct MovingCard {
    int card;
    SDL_Point mouseOffset;
};

class Stack;

class MovingStack
{
public:
    MovingStack();
    void draw();
    void clear();
    int getSize();
    int getCardAt(int index);
    Stack* getFromStack();
    void setFromStack(Stack* stack);
    void addMovingCard(MovingCard card);
    void updatePosition(int x, int y);
    void mouseDrag(int mouseX, int mouseY);
private:
    SDL_Rect firstCardRect;

    Stack* fromStack;
    std::vector<MovingCard> cards;
};

#endif // HEADER_MOVING