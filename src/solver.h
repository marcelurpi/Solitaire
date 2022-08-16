
#include "solitaire.h"

class Solver
{
public:
    Solver();
    Solver(Deck* deck, Stack* stacks, Completed* completed);
    void nextMove();
private:
    bool stackToCompleted();
    bool deckToCompleted();
    bool stackToStackUncovering();
    bool deckToStack();
    bool stackToStack();

    Deck* deck;
    Stack* stacks;
    Completed* completed;
    int toUncover[NUM_STACKS];
};