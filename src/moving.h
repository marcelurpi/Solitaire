
#ifndef HEADER_MOVING
#define HEADER_MOVING

#include "textureManager.h"
#include "stack.h"
#include <vector>

struct MovingCard {
    int card;
    SDL_Point mouseOffset;
};

enum class MovingFrom {
    Deck,
    Completed,
    Stack,
};

class Completed;
class Stack;

class MovingStack
{
public:
    MovingStack();
    void draw();
    void clear();
    int getSize();
    int getCardAt(int index);
    MovingFrom getMovingFrom();
    Completed* getFromCompleted();
    Stack* getFromStack();
    void setMovingFrom(MovingFrom movingFrom, Completed* completed, Stack* stack);
    void addMovingCard(MovingCard card);
    void updatePosition(int x, int y);
    void mouseDrag(int mouseX, int mouseY);
private:
    SDL_Rect firstCardRect;

    MovingFrom movingFrom;
    Completed* fromCompleted;
    Stack* fromStack;
    std::vector<MovingCard> cards;
};

#endif // HEADER_MOVING