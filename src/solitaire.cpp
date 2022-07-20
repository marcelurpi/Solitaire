
#include "solitaire.h"
#include <iostream>

Solitaire::Solitaire() : deckIndex(-1), spacePressed(false)
{
    srand(2727); //srand(time(0));
    resetBoard();

    topLeft = SDL_Point{-NUM_STACKS * 100 / 2, (-150 - NUM_STACKS * 20) / 2};
    backDeckRect = SDL_Rect{topLeft.x, topLeft.y, CARD_WIDTH, CARD_HEIGHT};
    frontDeckRect = SDL_Rect{topLeft.x + 100, topLeft.y, CARD_WIDTH, CARD_HEIGHT};
    for (int i = 0; i < 4; i++) {
        completedRects[i] = SDL_Rect{topLeft.x + 100 * (3 + i), topLeft.y, CARD_WIDTH, CARD_HEIGHT};
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        stacksRectsBase[i] = SDL_Rect{topLeft.x + 100 * i, topLeft.y + 150, CARD_WIDTH, CARD_HEIGHT};
        stacksRects[i] = std::vector<SDL_Rect>(i + 1);
        for (int j = 0; j < i + 1; j++) {
            stacksRects[i][j] = SDL_Rect(stacksRectsBase[i]);
            stacksRects[i][j].y += j * 20;
        }
    }
    movingCard.card = -1;
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
        TextureManager::Instance()->drawCard(cardsTopCompleted[i], &completedRects[i], false);
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        if (spacePressed) {
            for (int j = 0; j < stacksRects[i].size(); j++) {
                TextureManager::Instance()->drawCard(CARD_EMPTY, &stacksRects[i][j], false);
            }
        } else {
            for (int j = 0; j < cardsStacks[i].size(); j++) {
                int card = numCardsStacksHidden[i] > j ? CARD_BACK : cardsStacks[i][j];
                TextureManager::Instance()->drawCard(card, &stacksRects[i][j], false);
            }
        }
    }
    if (movingCard.card != -1) {
        TextureManager::Instance()->drawCard(movingCard.card, &movingCardRect, false);
    }
}

void Solitaire::mouseDown(int mouseX, int mouseY) 
{
    if (isMouseInsideRect(mouseX, mouseY, &backDeckRect)) {
        deckIndex = ((deckIndex + 2) % (cardsDeck.size() + 1)) - 1;
        return;
    }
    if (isMouseInsideRect(mouseX, mouseY, &frontDeckRect) && deckIndex != -1) {
        movingCard.card = cardsDeck[deckIndex];
        movingCard.stack = -1;
        cardsDeck.erase(cardsDeck.begin() + deckIndex);
        deckIndex--;
        return;
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        // for (int j = cardsStacks[i].size() - 1; j >= numCardsStacksHidden[i]; j--) {
        for (int j = cardsStacks[i].size() - 1; j == cardsStacks[i].size() - 1; j--) {
            if (isMouseInsideRect(mouseX, mouseY, &stacksRects[i][j])) {
                movingCard.card = cardsStacks[i][j];
                movingCard.stack = i;
                cardsStacks[i].pop_back();
                movingCardRect = SDL_Rect{topLeft.x + 100 * i, topLeft.y + 150 + 20 * j, CARD_WIDTH, CARD_HEIGHT};
                movingCard.mouseOffset.x = (topLeft.x + 100 * i) - mouseX;
                movingCard.mouseOffset.y = (topLeft.y + 150 + 20 * j) - mouseY;
                return;
            }
        }
    }
}

void Solitaire::mouseUp(int mouseX, int mouseY)
{
    if (movingCard.card == -1) return;

    for (int i = 0; i < 4; i++) {
        if (isMouseInsideRect(mouseX, mouseY, &completedRects[i]) &&
            cardCanBePlacedOnCompleted(movingCard.card, i)) 
        {
            cardsTopCompleted[i] = movingCard.card;

            if (movingCard.stack != -1 && numCardsStacksHidden[movingCard.stack] != 0) {
                stacksRects[movingCard.stack].pop_back();
                int cardsHidden = numCardsStacksHidden[movingCard.stack];
                if (cardsHidden > 0 && cardsStacks[movingCard.stack].size() == cardsHidden) {
                    numCardsStacksHidden[movingCard.stack]--;
                }
            }
            
            movingCard.card = -1;
            return;
        }
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        bool clickingValidStack = isMouseInsideRect(mouseX, mouseY, &stacksRects[i].back()) && movingCard.stack != i &&
            cardCanBePlacedOnStack(movingCard.card, i);
        bool clickingEmptyStack = isMouseInsideRect(mouseX, mouseY, &stacksRectsBase[i]) && 
            cardsStacks[i].size() == 0 && (movingCard.card % 14) == 12;
        if (clickingValidStack || clickingEmptyStack)
        {
            if (movingCard.stack != -1) {
                stacksRects[movingCard.stack].pop_back();
            }

            SDL_Rect stackCardRect(stacksRectsBase[i]);
            stackCardRect.y += cardsStacks[i].size() * 20;
            stacksRects[i].push_back(stackCardRect);

            cardsStacks[i].push_back(movingCard.card);
            if (movingCard.stack != -1 && numCardsStacksHidden[movingCard.stack] != 0) {
                int cardsHidden = numCardsStacksHidden[movingCard.stack];
                if (cardsHidden > 0 && cardsStacks[movingCard.stack].size() == cardsHidden) {
                    numCardsStacksHidden[movingCard.stack]--;
                }
            }
            movingCard.card = -1;
            return;
        }
    }
    if (movingCard.stack == -1) {
        deckIndex++;
        cardsDeck.insert(cardsDeck.begin() + deckIndex, movingCard.card);
    } else {
        cardsStacks[movingCard.stack].push_back(movingCard.card);
    }
    movingCard.card = -1;
}

void Solitaire::mouseDrag(int mouseX, int mouseY) 
{
    movingCardRect.x = mouseX + movingCard.mouseOffset.x;
    movingCardRect.y = mouseY + movingCard.mouseOffset.y;
}

void Solitaire::keyDown(SDL_Keysym keysym)
{
    if (keysym.sym == SDLK_SPACE) {
        spacePressed = true;
    }
}

void Solitaire::keyUp(SDL_Keysym keysym)
{
    if (keysym.sym == SDLK_SPACE) {
        spacePressed = false;
    }
}

void Solitaire::resetBoard()
{
    movingCard.card = -1;
    for (int i = 0; i < 4; i++) {
        cardsTopCompleted[i] = CARD_EMPTY;
    }
    cardsDeck = std::vector<int>(52);
    for (int i = 0; i < 52; i++) {
        cardsDeck[i] = (i / 13) * 14 + i % 13;
    }
    shuffle(cardsDeck);
    for (int i = 0; i < NUM_STACKS; i++) {
        numCardsStacksHidden[i] = i;
        cardsStacks[i] = std::vector<int>(i + 1);
        for (int j = 0; j < i + 1; j++) {
            cardsStacks[i][j] = cardsDeck.back();
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

bool Solitaire::cardCanBePlacedOnStack(int card, int stack) 
{
    int card2 = cardsStacks[stack].back();
    bool oppositeColor = card / 28 != card2 / 28;
    bool numberJustUnder = card % 14 == (card2 % 14) - 1;
    return oppositeColor && numberJustUnder;
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