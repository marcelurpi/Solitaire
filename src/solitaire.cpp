
#include "solitaire.h"
#include <iostream>

Solitaire::Solitaire()
{
    srand(2727); //srand(time(0));
    SDL_Point topLeft{-NUM_STACKS * 100 / 2, -50 - (150 + NUM_STACKS * 20) / 2};
    for (int i = 0; i < 4; i++) {
        completedRects[i] = SDL_Rect{topLeft.x + 100 * (3 + i), topLeft.y, CARD_WIDTH, CARD_HEIGHT};
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        stacks[i] = Stack(topLeft.x + 100 * i, topLeft.y + 150, &movingStack);
    }
    deck = Deck(topLeft.x, topLeft.y, &movingStack);
    resetBoard();
}

void Solitaire::drawBoard()
{
    deck.draw();
    for (int i = 0; i < 4; i++) {
        bool transparent = cardsTopCompleted[i] == CARD_EMPTY;
        TextureManager::Instance()->drawCard(cardsTopCompleted[i], &completedRects[i], transparent);
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        stacks[i].draw();
    }
    movingStack.draw();
}

void Solitaire::mouseDown(int mouseX, int mouseY) 
{
    if (deck.mouseDown(mouseX, mouseY)) {
        return;
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        if (stacks[i].mouseDown(mouseX, mouseY)) {
            return;
        }
    }
}

void Solitaire::mouseUp(int mouseX, int mouseY)
{
    if (movingStack.getSize() == 0) return;

    Stack* fromStack = movingStack.getFromStack();
    for (int i = 0; i < 4; i++) {
        if (isMouseInsideRect(mouseX, mouseY, &completedRects[i]) &&
            movingStack.getSize() == 1 &&
            cardCanBePlacedOnCompleted(movingStack.getCardAt(0), i)) 
        {
            cardsTopCompleted[i] = movingStack.getCardAt(0);
            if (fromStack != nullptr) {
                fromStack->uncoverCardIfPossible();
            }
            movingStack.clear();
            return;
        }
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        if (stacks[i].mouseUp(mouseX, mouseY)) {
            return;
        }
    }
    if (fromStack == nullptr) {
        if (deck.mouseUp(mouseX, mouseY)) {
            return;
        }
    } else {
        for (int i = 0; i < movingStack.getSize(); i++) {
            fromStack->addCard(movingStack.getCardAt(i));
        }
        movingStack.clear();
    }
}

void Solitaire::mouseDrag(int mouseX, int mouseY) 
{
    movingStack.mouseDrag(mouseX, mouseY);
}

void Solitaire::keyDown(SDL_Keysym keysym)
{

}

void Solitaire::keyUp(SDL_Keysym keysym)
{
    if (keysym.sym == SDLK_r) {
        srand(2727);
        resetBoard();
    }
}

void Solitaire::resetBoard()
{
    movingStack.clear();
    for (int i = 0; i < 4; i++) {
        cardsTopCompleted[i] = CARD_EMPTY;
    }
    deck.reset();
    for (int i = 0; i < NUM_STACKS; i++) {

        stacks[i].setNumHiddenCards(i);
        for (int j = 0; j < i + 1; j++) {
            stacks[i].addCard(deck.popCard());
        }
    }
}

bool Solitaire::cardCanBePlacedOnCompleted(int card, int stack) 
{
    int card2 = cardsTopCompleted[stack];
    if (card2 == CARD_EMPTY) return card % 14 == 0;

    bool sameSuit = card / 14 == card2 / 14;
    bool numberJustOver = card % 14 == (card2 % 14) + 1;
    return sameSuit && numberJustOver;
}

bool Solitaire::isMouseInsideRect(int mouseX, int mouseY, SDL_Rect* rect)
{
    bool insideX = mouseX >= rect->x - rect->w / 2 && mouseX <= rect->x + rect->w / 2;
    return insideX && mouseY >= rect->y - rect->h / 2 && mouseY <= rect->y + rect->h / 2;
}