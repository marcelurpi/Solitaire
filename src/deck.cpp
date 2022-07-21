
#include "deck.h"

Deck::Deck() {}

Deck::Deck(int baseX, int baseY, MovingStack* moving)
{
    this->moving = moving;
    backDeckRect = SDL_Rect{baseX, baseY, CARD_WIDTH, CARD_HEIGHT};
    frontDeckRect = SDL_Rect{baseX + 100, baseY, CARD_WIDTH, CARD_HEIGHT};
    cards = std::vector<int>(52);
}

void Deck::draw()
{
    if (index == cards.size() - 1) {
        TextureManager::Instance()->drawCard(CARD_EMPTY, &backDeckRect, true);
    } else {
        TextureManager::Instance()->drawCard(CARD_BACK, &backDeckRect, false);
    }
    if (index != -1) {
        TextureManager::Instance()->drawCard(cards[index], &frontDeckRect, false);
    }
}

void Deck::reset()
{
    index = -1;
    cards.empty();
    for (int i = 0; i < 52; i++) {
        cards[i] = (i / 13) * 14 + i % 13;
    }
    shuffle();
}

int Deck::popCard()
{
    int card = cards.back();
    cards.pop_back();
    return card;
}

bool Deck::mouseDown(int mouseX, int mouseY)
{
    if (isMouseInsideRect(mouseX, mouseY, &backDeckRect)) {
        index = ((index + 2) % (cards.size() + 1)) - 1;
        return true;
    }
    if (isMouseInsideRect(mouseX, mouseY, &frontDeckRect) && index != -1) {
        SDL_Point mouseOffset{frontDeckRect.x - mouseX, frontDeckRect.y - mouseY};
        MovingCard movingCard{cards[index], mouseOffset};
        moving->addMovingCard(movingCard);
        cards.erase(cards.begin() + index);
        index--;
        moving->setFromStack(nullptr);
        moving->updatePosition(frontDeckRect.x, frontDeckRect.y);
        return true;
    }
    return false;
}

bool Deck::mouseUp(int mouseX, int mouseY)
{
    index++;
    cards.insert(cards.begin() + index, moving->getCardAt(0));
    moving->clear();
    return true;
}

void Deck::shuffle()
{
    for (int i = cards.size() - 1; i >= 1; i--) {
        int j = rand() % i;
        int tmp = cards[j];
        cards[j] = cards[i];
        cards[i] = tmp;
    }
}

bool Deck::isMouseInsideRect(int mouseX, int mouseY, SDL_Rect* rect)
{
    bool insideX = mouseX >= rect->x - rect->w / 2 && mouseX <= rect->x + rect->w / 2;
    return insideX && mouseY >= rect->y - rect->h / 2 && mouseY <= rect->y + rect->h / 2;
}