
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
        Resources::Instance()->drawCard(CARD_EMPTY, &backDeckRect, true);
    } else {
        Resources::Instance()->drawCard(CARD_BACK, &backDeckRect, false);
    }
    if (index != -1) {
        Resources::Instance()->drawCard(cards[index], &frontDeckRect, false);
    }
}

void Deck::reset()
{
    index = -1;
    cards = std::vector<int>(52);
    for (int i = 0; i < 52; i++) {
        cards[i] = (i / 13) * 14 + i % 13;
    }
    shuffle();
}

void Deck::nextCard()
{
    if (index == cards.size() - 1) {
        index = -1;
    } else {
        index++;
    }
    Resources::Instance()->playSound(Sound::CardPlace);
}

int Deck::popCard()
{
    int card = cards.back();
    cards.pop_back();
    return card;
}

int Deck::getIndex()
{
    return index;
}

void Deck::removeAtIndex()
{
    cards.erase(cards.begin() + index);
    index--;
}

std::vector<int> Deck::getCards()
{
    return cards;
}

void Deck::returnCard()
{
    index++;
    cards.insert(cards.begin() + index, moving->getCardAt(0));
    moving->clear();
}

bool Deck::mouseDown(int mouseX, int mouseY)
{
    if (isMouseInsideRect(mouseX, mouseY, &backDeckRect)) {
        nextCard();
        return true;
    }
    if (isMouseInsideRect(mouseX, mouseY, &frontDeckRect) && index != -1) {
        SDL_Point mouseOffset{frontDeckRect.x - mouseX, frontDeckRect.y - mouseY};
        MovingCard movingCard{cards[index], mouseOffset};
        moving->addMovingCard(movingCard);
        removeAtIndex();
        moving->setMovingFrom(MovingFrom::Deck, nullptr, nullptr);
        moving->updatePosition(frontDeckRect.x, frontDeckRect.y);
        return true;
    }
    return false;
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