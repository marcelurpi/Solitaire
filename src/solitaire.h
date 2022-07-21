
#ifndef HEADER_SOLITAIRE
#define HEADER_SOLITAIRE

#include <vector>
#include <time.h>
#include "textureManager.h"

const int NUM_STACKS = 7;
const int CARD_WIDTH = 84;
const int CARD_HEIGHT = 120;

const int CARD_EMPTY = 13;
const int CARD_BACK = 27;

struct MovingCard {
    int card;
    SDL_Point mouseOffset;
};

struct MovingStack {
    int stack;
    std::vector<MovingCard> cards;
};

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
    bool cardCanBePlacedOnStack(int card, int stack);
    bool cardCanBePlacedOnCompleted(int card, int stack);
    bool isMouseInsideRect(int mouseX, int mouseY, SDL_Rect* rect);

    SDL_Point topLeft;
    SDL_Rect movingCardRect;
    SDL_Rect backDeckRect;
    SDL_Rect frontDeckRect;
    SDL_Rect completedRects[4];
    SDL_Rect stacksRectsBase[NUM_STACKS];
    std::vector<SDL_Rect> stacksRects[NUM_STACKS];

    bool spacePressed;
    int deckIndex;
    int numCardsStacksHidden[NUM_STACKS];
    MovingStack movingStack;
    int cardsTopCompleted[4];
    std::vector<int> cardsDeck;
    std::vector<int> cardsStacks[NUM_STACKS];
};

#endif // HEADER_SOLITAIRE