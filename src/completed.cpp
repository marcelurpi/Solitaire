
#include "completed.h"

Completed::Completed() {}

Completed::Completed(int x, int y, MovingStack* moving)
{
    this->moving = moving;
    rect = SDL_Rect{x, y, CARD_WIDTH, CARD_HEIGHT};
}

void Completed::draw()
{
    bool transparent = topCompleted == CARD_EMPTY;
    Resources::Instance()->drawCard(topCompleted, &rect, transparent);
}

void Completed::reset()
{
    topCompleted = CARD_EMPTY;
}

void Completed::returnCard()
{
    placeCardOnTop(moving->getCardAt(0));
    Stack* fromStack = moving->getFromStack();
    if (fromStack != nullptr) {
        fromStack->uncoverCardIfPossible();
    }
    moving->clear();
}

bool Completed::mouseDown(int mouseX, int mouseY)
{
    if (isMouseInsideRect(mouseX, mouseY, &rect) && topCompleted != CARD_EMPTY) {
        SDL_Point mouseOffset{rect.x - mouseX, rect.y - mouseY};
        MovingCard movingCard{topCompleted, mouseOffset};
        moving->addMovingCard(movingCard);
        moving->setMovingFrom(MovingFrom::Completed, this, nullptr);
        moving->updatePosition(rect.x, rect.y);
        topCompleted = topCompleted % 14 == 0 ? CARD_EMPTY : topCompleted - 1;
        return true;
    }
    return false;
}

bool Completed::mouseUp(int mouseX, int mouseY)
{
    if (isMouseInsideRect(mouseX, mouseY, &rect) &&
        moving->getSize() == 1 &&
        cardCanBePlacedOnTop(moving->getCardAt(0))) 
    {
        returnCard();
        return true;
    }
    return false;
}

bool Completed::cardCanBePlacedOnTop(int card)
{
    if (topCompleted == CARD_EMPTY) return card % 14 == 0;

    bool sameSuit = card / 14 == topCompleted / 14;
    bool numberJustOver = card % 14 == (topCompleted % 14) + 1;
    return sameSuit && numberJustOver;
}

int Completed::getCardOnTop()
{
    return topCompleted;
}

void Completed::placeCardOnTop(int card)
{
    topCompleted = card;
    Resources::Instance()->playSound(Sound::CardShove);
}
