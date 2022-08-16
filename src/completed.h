
#ifndef HEADER_COMPLETED
#define HEADER_COMPLETED

#include "resources.h"
#include "moving.h"

class MovingStack;

class Completed
{
public:
    Completed();
    Completed(int x, int y, MovingStack* moving);
    void draw();
    void reset();
    void returnCard();
    bool mouseDown(int mouseX, int mouseY);
    bool mouseUp(int mouseX, int mouseY);
    bool cardCanBePlacedOnTop(int card);
    int getCardOnTop();
    void placeCardOnTop(int card);
private:
    SDL_Rect rect;
    int topCompleted;
    MovingStack* moving;
};

#endif // HEADER_COMPLETED