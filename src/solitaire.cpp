
#include "solitaire.h"
#include <iostream>

Solitaire::Solitaire()
{
    srand(2727); //srand(time(0));
    
    SDL_Point topLeft{-NUM_STACKS * 100 / 2, -50 - (150 + NUM_STACKS * 20) / 2};
    backDeckRect = SDL_Rect{topLeft.x, topLeft.y, CARD_WIDTH, CARD_HEIGHT};
    frontDeckRect = SDL_Rect{topLeft.x + 100, topLeft.y, CARD_WIDTH, CARD_HEIGHT};
    for (int i = 0; i < 4; i++) {
        completedRects[i] = SDL_Rect{topLeft.x + 100 * (3 + i), topLeft.y, CARD_WIDTH, CARD_HEIGHT};
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        stacks[i] = Stack(topLeft.x + 100 * i, topLeft.y + 150, &movingStack);
    }

    resetBoard();
}

void Solitaire::drawBoard()
{
    if (deckIndex == cardsDeck.size() - 1) {
        TextureManager::Instance()->drawCard(CARD_EMPTY, &backDeckRect, true);
    } else {
        TextureManager::Instance()->drawCard(CARD_BACK, &backDeckRect, false);
    }
    if (deckIndex != -1) {
        TextureManager::Instance()->drawCard(cardsDeck[deckIndex], &frontDeckRect, false);
    }
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
    if (isMouseInsideRect(mouseX, mouseY, &backDeckRect)) {
        deckIndex = ((deckIndex + 2) % (cardsDeck.size() + 1)) - 1;
        return;
    }
    if (isMouseInsideRect(mouseX, mouseY, &frontDeckRect) && deckIndex != -1) {
        SDL_Point mouseOffset{frontDeckRect.x - mouseX, frontDeckRect.y - mouseY};
        MovingCard movingCard{cardsDeck[deckIndex], mouseOffset};
        movingStack.addMovingCard(movingCard);
        cardsDeck.erase(cardsDeck.begin() + deckIndex);
        deckIndex--;
        movingStack.setFromStack(nullptr);
        movingStack.updatePosition(frontDeckRect.x, frontDeckRect.y);
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
        deckIndex++;
        cardsDeck.insert(cardsDeck.begin() + deckIndex, movingStack.getCardAt(0));
    } else {
        for (int i = 0; i < movingStack.getSize(); i++) {
            fromStack->addCard(movingStack.getCardAt(i));
        }
    }
    movingStack.clear();
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
    deckIndex = -1;
    cardsDeck = std::vector<int>(52);
    for (int i = 0; i < 52; i++) {
        cardsDeck[i] = (i / 13) * 14 + i % 13;
    }
    shuffle(cardsDeck);
    for (int i = 0; i < NUM_STACKS; i++) {

        stacks[i].setNumHiddenCards(i);
        for (int j = 0; j < i + 1; j++) {
            stacks[i].addCard(cardsDeck.back());
            cardsDeck.pop_back();
        }
    }
}

void Solitaire::shuffle(std::vector<int>& vector) 
{
    for (int i = vector.size() - 1; i >= 1; i--) {
        int j = rand() % i;
        int tmp = vector[j];
        vector[j] = vector[i];
        vector[i] = tmp;
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