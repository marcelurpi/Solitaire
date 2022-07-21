
#include "solitaire.h"
#include <iostream>

Solitaire::Solitaire()
{
    srand(2727); //srand(time(0));
    
    topLeft = SDL_Point{-NUM_STACKS * 100 / 2, (-150 - NUM_STACKS * 20) / 2};
    backDeckRect = SDL_Rect{topLeft.x, topLeft.y, CARD_WIDTH, CARD_HEIGHT};
    frontDeckRect = SDL_Rect{topLeft.x + 100, topLeft.y, CARD_WIDTH, CARD_HEIGHT};
    for (int i = 0; i < 4; i++) {
        completedRects[i] = SDL_Rect{topLeft.x + 100 * (3 + i), topLeft.y, CARD_WIDTH, CARD_HEIGHT};
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        stacksRectsBase[i] = SDL_Rect{topLeft.x + 100 * i, topLeft.y + 150, CARD_WIDTH, CARD_HEIGHT};
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
        if (spacePressed) {
            if (cardsStacks[i].size() == 0) {
                TextureManager::Instance()->drawCard(CARD_EMPTY, &stacksRectsBase[i], true);
            } else {
                for (int j = 0; j < cardsStacks[i].size(); j++) {
                    TextureManager::Instance()->drawCard(CARD_EMPTY, &stacksRects[i][j], false);
                }
            }
        } else {
            for (int j = 0; j < cardsStacks[i].size(); j++) {
                int card = numCardsStacksHidden[i] > j ? CARD_BACK : cardsStacks[i][j];
                TextureManager::Instance()->drawCard(card, &stacksRects[i][j], false);
            }
        }
    }
    for (int i = 0; i < movingStack.cards.size(); i++) {
        SDL_Rect movingStackCardRect(movingCardRect);
        movingStackCardRect.y += i * 20;
        TextureManager::Instance()->drawCard(movingStack.cards[i].card, &movingStackCardRect, false);
    }
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
        movingStack.cards.push_back(movingCard);
        cardsDeck.erase(cardsDeck.begin() + deckIndex);
        deckIndex--;
        movingStack.stack = -1;
        movingCardRect = SDL_Rect{frontDeckRect.x, frontDeckRect.y, CARD_WIDTH, CARD_HEIGHT};
        return;
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        for (int j = cardsStacks[i].size() - 1; j >= numCardsStacksHidden[i]; j--) {
            if (isMouseInsideRect(mouseX, mouseY, &stacksRects[i][j])) {
                for (int k = j; k < cardsStacks[i].size(); k++) {
                    SDL_Point mouseOffset{stacksRects[i][k].x - mouseX, stacksRects[i][k].y - mouseY};
                    MovingCard movingCard{cardsStacks[i][k], mouseOffset};
                    movingStack.cards.push_back(movingCard);
                }
                int cardsStacksSize = cardsStacks[i].size();
                for (int k = j; k < cardsStacksSize; k++) {
                    cardsStacks[i].pop_back();
                }
                movingStack.stack = i;
                movingCardRect = SDL_Rect{stacksRects[i][j].x, stacksRects[i][j].y, CARD_WIDTH, CARD_HEIGHT};
                return;
            }
        }
    }
}

void Solitaire::mouseUp(int mouseX, int mouseY)
{
    if (movingStack.cards.size() == 0) return;

    for (int i = 0; i < 4; i++) {
        if (isMouseInsideRect(mouseX, mouseY, &completedRects[i]) &&
            movingStack.cards.size() == 1 &&
            cardCanBePlacedOnCompleted(movingStack.cards[0].card, i)) 
        {
            cardsTopCompleted[i] = movingStack.cards[0].card;

            if (movingStack.stack != -1) {
                stacksRects[movingStack.stack].pop_back();
                int cardsHidden = numCardsStacksHidden[movingStack.stack];
                if (cardsHidden != 0 && cardsStacks[movingStack.stack].size() == cardsHidden) {
                    numCardsStacksHidden[movingStack.stack]--;
                }
            }
            
            movingStack.cards.pop_back();
            return;
        }
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        bool clickingValidStack = isMouseInsideRect(mouseX, mouseY, &stacksRects[i].back()) && movingStack.stack != i &&
            cardCanBePlacedOnStack(movingStack.cards[0].card, i);
        bool clickingEmptyStack = isMouseInsideRect(mouseX, mouseY, &stacksRectsBase[i]) && 
            cardsStacks[i].size() == 0 && (movingStack.cards[0].card % 14) == 12;
        if (clickingValidStack || clickingEmptyStack)
        {
            if (movingStack.stack != -1) {
                for (int j = 0; j < movingStack.cards.size(); j++) {
                    stacksRects[movingStack.stack].pop_back();
                }
            }

            for (int j = 0; j < movingStack.cards.size(); j++) {
                SDL_Rect stackCardRect(stacksRectsBase[i]);
                stackCardRect.y += cardsStacks[i].size() * 20;
                stacksRects[i].push_back(stackCardRect);
                cardsStacks[i].push_back(movingStack.cards[j].card);
            }
        
            if (movingStack.stack != -1) {
                int cardsHidden = numCardsStacksHidden[movingStack.stack];
                if (cardsHidden != 0 && cardsStacks[movingStack.stack].size() == cardsHidden) {
                    numCardsStacksHidden[movingStack.stack]--;
                }
            }

            while (movingStack.cards.size() != 0) {
                movingStack.cards.pop_back();
            }
            return;
        }
    }
    if (movingStack.stack == -1) {
        deckIndex++;
        cardsDeck.insert(cardsDeck.begin() + deckIndex, movingStack.cards[0].card);
    } else {
        for (int i = 0; i < movingStack.cards.size(); i++) {
            cardsStacks[movingStack.stack].push_back(movingStack.cards[i].card);
        }
    }
    while (movingStack.cards.size() != 0) {
        movingStack.cards.pop_back();
    }
}

void Solitaire::mouseDrag(int mouseX, int mouseY) 
{
    if (movingStack.cards.size() != 0) {
        movingCardRect.x = mouseX + movingStack.cards[0].mouseOffset.x;
        movingCardRect.y = mouseY + movingStack.cards[0].mouseOffset.y;
    }
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
    if (keysym.sym == SDLK_r) {
        srand(2727);
        resetBoard();
    }
}

void Solitaire::resetBoard()
{
    movingStack.stack = -1;
    movingStack.cards = std::vector<MovingCard>(0);
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
        numCardsStacksHidden[i] = i;
        cardsStacks[i] = std::vector<int>(i + 1);
        for (int j = 0; j < i + 1; j++) {
            cardsStacks[i][j] = cardsDeck.back();
            cardsDeck.pop_back();
        }
        stacksRects[i] = std::vector<SDL_Rect>(i + 1);
        for (int j = 0; j < i + 1; j++) {
            stacksRects[i][j] = SDL_Rect(stacksRectsBase[i]);
            stacksRects[i][j].y += j * 20;
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