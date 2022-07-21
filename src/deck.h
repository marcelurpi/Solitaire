
#ifndef HEADER_DECK
#define HEADER_DECK

#include "textureManager.h"
#include "moving.h"
#include <vector>

class Deck
{
public:
    Deck();
    Deck(int baseX, int baseY, MovingStack* moving);
    void draw();
    void reset();
    int popCard();
    bool mouseDown(int mouseX, int mouseY);
    bool mouseUp(int mouseX, int mouseY);
private:
    void shuffle();
    bool isMouseInsideRect(int mouseX, int mouseY, SDL_Rect* rect);

    SDL_Rect backDeckRect;
    SDL_Rect frontDeckRect;

    int index;
    std::vector<int> cards;
    MovingStack* moving;
};

#endif // HEADER_DECK