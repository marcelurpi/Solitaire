
#include "stack.h"

Stack::Stack() {}

Stack::Stack(int baseX, int baseY, MovingStack* moving)
{
    this->moving = moving;
    for (int i = 0; i < MAX_STACK_SIZE; i++) {
        cardRects[i] = SDL_Rect{baseX, baseY + i * 20, CARD_WIDTH, CARD_HEIGHT};
    }
}

void Stack::draw()
{
    for (int i = 0; i < cards.size(); i++) {
        int card = numHiddenCards > i ? CARD_BACK : cards[i];
        Resources::Instance()->drawCard(card, &cardRects[i], false);
    }
}

void Stack::reset()
{
    cards = std::vector<int>(0);
}

int Stack::getNumHiddenCards()
{
    return numHiddenCards;
}

void Stack::setNumHiddenCards(int hidden)
{
    numHiddenCards = hidden;
}

void Stack::addCard(int card) 
{
    cards.push_back(card);
    Resources::Instance()->playSound(Sound::CardPlace);
}

void Stack::uncoverCardIfPossible()
{
    if (numHiddenCards != 0 && cards.size() == numHiddenCards) {
        numHiddenCards--;
    }
}

bool Stack::mouseDown(int mouseX, int mouseY)
{
    for (int j = cards.size() - 1; j >= numHiddenCards; j--) {
        if (isMouseInsideRect(mouseX, mouseY, &cardRects[j])) {
            for (int k = j; k < cards.size(); k++) {
                SDL_Point mouseOffset{cardRects[k].x - mouseX, cardRects[k].y - mouseY};
                MovingCard movingCard{cards[k], mouseOffset};
                moving->addMovingCard(movingCard);
            }
            int cardsStacksSize = cards.size();
            for (int k = j; k < cardsStacksSize; k++) {
                cards.pop_back();
            }
            moving->setMovingFrom(MovingFrom::Stack, nullptr, this);
            moving->updatePosition(cardRects[j].x, cardRects[j].y);
            return true;
        }
    }
    return false;
}

bool Stack::mouseUp(int mouseX, int mouseY)
{
    SDL_Rect rect = cards.size() == 0 ? cardRects[0] : cardRects[cards.size() - 1];
    if (isMouseInsideRect(mouseX, mouseY, &rect) && cardCanBePlacedOnStack(moving->getCardAt(0)))
    {
        for (int j = 0; j < moving->getSize(); j++) {
            addCard(moving->getCardAt(j));
        }
        Stack* movingFromStack = moving->getFromStack();
        if (movingFromStack != nullptr) {
            movingFromStack->uncoverCardIfPossible();
        }
        moving->clear();
        return true;
    }
    return false;
}

std::vector<int> Stack::getCards()
{
    return cards;
}

void Stack::removeCards(int num)
{
    for (int i = 0; i < num; i++) {
        cards.pop_back();
    }
    uncoverCardIfPossible();
}

bool Stack::cardCanBePlacedOnStack(int card) 
{
    if (cards.size() == 0) return card % 14 == 12;
    int card2 = cards.back();
    bool oppositeColor = card / 28 != card2 / 28;
    bool numberJustUnder = card % 14 == (card2 % 14) - 1;
    return oppositeColor && numberJustUnder;
}