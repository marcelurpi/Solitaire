
#include "solitaire.h"
#include <iostream>

Solitaire::Solitaire()
{
    srand(2727); //srand(time(0));
    SDL_Point topLeft{-NUM_STACKS * 100 / 2, -50 - (150 + NUM_STACKS * 20) / 2};
    for (int i = 0; i < 4; i++) {
        completed[i] = Completed(topLeft.x + 100 * (3 + i), topLeft.y, &movingStack);
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        stacks[i] = Stack(topLeft.x + 100 * i, topLeft.y + 150, &movingStack);
    }
    deck = Deck(topLeft.x, topLeft.y, &movingStack);
    reset();
}

void Solitaire::draw()
{
    deck.draw();
    for (int i = 0; i < 4; i++) {
        completed[i].draw();
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
    for (int i = 0; i < 4; i++) {
        if (completed[i].mouseDown(mouseX, mouseY)) {
            return;
        }
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

    for (int i = 0; i < 4; i++) {
        if (completed[i].mouseUp(mouseX, mouseY)) {
            return;
        }
    }
    for (int i = 0; i < NUM_STACKS; i++) {
        if (stacks[i].mouseUp(mouseX, mouseY)) {
            return;
        }
    }
    MovingFrom movingFrom = movingStack.getMovingFrom();
    if (movingFrom == MovingFrom::Deck) {
        deck.returnCard();
    } else if (movingFrom == MovingFrom::Completed) {
        movingStack.getFromCompleted()->returnCard();
    } else {
        Stack* fromStack = movingStack.getFromStack();
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
        reset();
    }
}

void Solitaire::reset()
{
    movingStack.clear();
    for (int i = 0; i < 4; i++) {
        completed[i].reset();
    }
    deck.reset();
    for (int i = 0; i < NUM_STACKS; i++) {
        stacks[i].reset();
        stacks[i].setNumHiddenCards(i);
        for (int j = 0; j < i + 1; j++) {
            stacks[i].addCard(deck.popCard());
        }
    }
}