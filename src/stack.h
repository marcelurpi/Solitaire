
#ifndef HEADER_STACK
#define HEADER_STACK

#include <vector>
#include "moving.h"

class MovingStack;

const int MAX_STACK_SIZE = NUM_STACKS - 1 + 13;

class Stack {
public:
    Stack();
    Stack(int baseX, int baseY, MovingStack* moving);
    void draw();
    void reset();
    void setNumHiddenCards(int hidden);
    void addCard(int card);
    void uncoverCardIfPossible();
    bool mouseDown(int mouseX, int mouseY);
    bool mouseUp(int mouseX, int mouseY);
private:
    bool cardCanBePlacedOnStack(int card);
    bool isMouseInsideRect(int mouseX, int mouseY, SDL_Rect* rect);

    int index;
    MovingStack* moving;
    SDL_Rect cardRects[MAX_STACK_SIZE];
    int numHiddenCards;
    std::vector<int> cards;
};

#endif // HEADER_STACK