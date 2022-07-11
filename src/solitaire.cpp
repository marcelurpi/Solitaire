
#include "solitaire.h"

Solitaire::Solitaire()
{
    srand(2727); //srand(time(0));
    resetBoard();

    topLeft = SDL_Point{-NUM_STACKS * 100 / 2, (-150 - NUM_STACKS * 20) / 2};
    backDeckRect = SDL_Rect{topLeft.x, topLeft.y, 84, 128};
    frontDeckRect = SDL_Rect{topLeft.x + 100, topLeft.y, 84, 128};
    for (int i = 0; i < 4; i++) {
        completedRects[i] = SDL_Rect{topLeft.x + 100 * (3 + i), topLeft.y, 84, 128};
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        stacksRects[i] = SDL_Rect{topLeft.x + 100 * i, topLeft.y + 150, 84, 128};
    }

    movingCard.card = -1;
}

void Solitaire::drawBoard()
{
    TextureManager::Instance()->drawCard(27, &backDeckRect);
    // TextureManager::Instance()->drawCard(0, &frontDeckRect);
    for (int i = 0; i < 4; i++) {
        TextureManager::Instance()->drawCard(cardsTopCompleted[i], &completedRects[i]);
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        for (int j = 0; j < cardsStacks[i].size(); j++) {
            SDL_Rect cardRect(stacksRects[i]);
            cardRect.y += j * 20;
            int card = numCardsStacksHidden[i] > j ? 27 : cardsStacks[i][j];
            TextureManager::Instance()->drawCard(card, &cardRect);
        }
    }
    if (movingCard.card != -1) {
        TextureManager::Instance()->drawCard(movingCard.card, &movingCardRect);
    }
}

void Solitaire::mouseDown(int mouseX, int mouseY) 
{
    for (int i = 0; i < NUM_STACKS; i++) {
        for (int j = cardsStacks[i].size() - 1; j >= numCardsStacksHidden[i]; j--) {
            SDL_Rect cardRect(stacksRects[i]);
            cardRect.y += j * 20;
            if (isMouseInsideRect(mouseX, mouseY, &cardRect)) {
                movingCard.card = cardsStacks[i][j];
                movingCard.stack = i;
                cardsStacks[i].pop_back();
                movingCardRect = SDL_Rect{topLeft.x + 100 * i, topLeft.y + 150 + 20 * j, 84, 128};
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
        bool mouseInside = isMouseInsideRect(mouseX, mouseY, &completedRects[i]);
        if (mouseInside) {
            cardsTopCompleted[i] = movingCard.card;
            movingCard.card = -1;
            return;
        }
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        bool mouseInside = isMouseInsideRect(mouseX, mouseY, &stacksRects[i]);
        if (mouseInside && movingCard.stack != i) {

        }
    }
    cardsStacks[movingCard.stack].push_back(movingCard.card);
    movingCard.card = -1;
}

void Solitaire::mouseDrag(int mouseX, int mouseY) 
{
    movingCardRect.x = mouseX + movingCard.mouseOffset.x;
    movingCardRect.y = mouseY + movingCard.mouseOffset.y;
}

void Solitaire::resetBoard()
{
    movingCard.card = -1;
    for (int i = 0; i < 4; i++) {
        cardsTopCompleted[i] = 13;
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

void Solitaire::shuffle(std::vector<int>& vector) {
    for (int i = vector.size() - 1; i >= 1; i--) {
        int j = rand() % i;
        int tmp = vector[j];
        vector[j] = vector[i];
        vector[i] = tmp;
    }
}

bool Solitaire::isMouseInsideRect(int mouseX, int mouseY, SDL_Rect* rect)
{
    bool insideX = mouseX >= rect->x - rect->w / 2 && mouseX <= rect->x + rect->w / 2;
    return insideX && mouseY >= rect->y - rect->h / 2 && mouseY <= rect->y + rect->h / 2;
}