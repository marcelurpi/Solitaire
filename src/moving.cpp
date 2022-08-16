
#include "moving.h"

MovingStack::MovingStack()
{
    firstCardRect = SDL_Rect{0, 0, CARD_WIDTH, CARD_HEIGHT};
}

void MovingStack::draw()
{
    for (int i = 0; i < cards.size(); i++) {
        SDL_Rect cardRect(firstCardRect);
        cardRect.y += i * 20;
        Resources::Instance()->drawCard(cards[i].card, &cardRect, false);
    }
}

void MovingStack::clear()
{
    fromStack = nullptr;
    fromCompleted = nullptr;
    cards.clear();
}

int MovingStack::getSize()
{
    return cards.size();
}

int MovingStack::getCardAt(int index) 
{
    return cards[index].card;
}

MovingFrom MovingStack::getMovingFrom()
{
    return movingFrom;
}

Completed* MovingStack::getFromCompleted()
{
    return fromCompleted;
}

Stack* MovingStack::getFromStack()
{
    return fromStack;
}

void MovingStack::setMovingFrom(MovingFrom movingFrom, Completed* completed, Stack* stack)
{
    this->movingFrom = movingFrom;
    fromCompleted = completed;
    fromStack = stack;
}

void MovingStack::addMovingCard(MovingCard card)
{
    cards.push_back(card);
}

void MovingStack::updatePosition(int x, int y)
{
    firstCardRect.x = x;
    firstCardRect.y = y;
}

void MovingStack::mouseDrag(int mouseX, int mouseY) 
{
    if (cards.size() != 0) {
        firstCardRect.x = mouseX + cards[0].mouseOffset.x;
        firstCardRect.y = mouseY + cards[0].mouseOffset.y;
    }
}