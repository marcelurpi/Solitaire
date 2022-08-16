
#ifndef HEADER_DECK
#define HEADER_DECK

#include "textureManager.h"
#include "moving.h"
#include "utils.h"
#include <vector>

class Deck
{
public:
    Deck();
    Deck(int baseX, int baseY, MovingStack* moving);
    void draw();
    void reset();
    void nextCard();
    int popCard();
    int getIndex();
    void removeAtIndex();
    std::vector<int> getCards();
    void returnCard();
    bool mouseDown(int mouseX, int mouseY);
private:
    void shuffle();

    SDL_Rect backDeckRect;
    SDL_Rect frontDeckRect;

    int index;
    std::vector<int> cards;
    MovingStack* moving;
};

#endif // HEADER_DECK