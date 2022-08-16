
#include "solver.h"
#include <algorithm>

Solver::Solver() {}

Solver::Solver(Deck* deck, Stack* stacks, Completed* completed)
{
    this->deck = deck;
    this->stacks = stacks;
    this->completed = completed;
}

void Solver::nextMove()
{
    if (deck->getCards().size() != 0 && deck->getIndex() == -1) {
        deck->nextCard();
        return;
    }
    if (stackToCompleted()) {
        return;
    }
    if (deckToCompleted()) {
        return;
    }
    if (stackToStackUncovering()) {
        return;
    }
    if (deckToStack()) {
        return;
    }
    if (stackToStack()) {
        return;
    }
}

bool Solver::stackToCompleted()
{
    for (int i = 0; i < NUM_STACKS; i++) {
        std::vector<int> cards = stacks[i].getCards();
        if (cards.size() == 0) continue;
        int card = cards.back();
        for (int j = 0; j < 4; j++) {
            if (completed[j].cardCanBePlacedOnTop(card)) {
                stacks[i].removeCards(1);
                completed[j].placeCardOnTop(card);
                return true;
            }
        }
    }
    return false;
}

bool Solver::deckToCompleted()
{
    std::vector<int> deckCards = deck->getCards();
    for (int i = 0; i < deckCards.size(); i++) {
        int offsetIndex = (deck->getIndex() + i) % deckCards.size();
        for (int j = 0; j < 4; j++) {
            if (completed[j].cardCanBePlacedOnTop(deckCards[offsetIndex])) {
                if (i != 0) {
                    deck->nextCard();
                } else {
                    deck->removeAtIndex();
                    completed[j].placeCardOnTop(deckCards[offsetIndex]);
                }
                return true;
            }
        }
    }
    return false;
}

bool Solver::stackToStackUncovering()
{
    std::vector<int> order(0);
    int hiddens[NUM_STACKS];
    for (int i = 0; i < NUM_STACKS; i++) {
        hiddens[i] = stacks[i].getNumHiddenCards();
        if (hiddens[i] == 0) continue;
        order.push_back(i);
    }
    std::sort(order.begin(), order.end(), [hiddens](int a, int b){
        return hiddens[a] > hiddens[b];
    });
    for (int i = 0; i < order.size(); i++) {
        int hidden = hiddens[order[i]]; 
        std::vector<int> cards = stacks[order[i]].getCards();
        for (int j = 0; j < NUM_STACKS; j++) {
            if (stacks[j].cardCanBePlacedOnStack(cards[hidden])) {
                stacks[order[i]].removeCards(cards.size() - hidden);
                for (int k2 = 0; k2 < cards.size() - hidden; k2++) {
                    stacks[j].addCard(cards[hidden + k2]);
                }
                return true;
            }
        }
    }
    return false;
}

bool Solver::deckToStack()
{
    std::vector<int> deckCards = deck->getCards();
    for (int i = 0; i < deckCards.size(); i++) {
        int offsetIndex = (deck->getIndex() + i) % deckCards.size();
        for (int j = 0; j < NUM_STACKS; j++) {
            if (stacks[j].cardCanBePlacedOnStack(deckCards[offsetIndex])) {
                if (i != 0) {
                    deck->nextCard();
                } else {
                    deck->removeAtIndex();
                    stacks[j].addCard(deckCards[offsetIndex]);
                }
                return true;
            }
        }
    }
    return false;
}

bool Solver::stackToStack()
{
    for (int i = 0; i < NUM_STACKS; i++) {
        std::vector<int> cards = stacks[i].getCards();
        for (int j = 0; j < NUM_STACKS; j++) {
            int numMovableCards = cards.size() - stacks[i].getNumHiddenCards();
            for (int k = 0; k < numMovableCards; k++) {
                int index = cards.size() - 1 - k;
                if (stacks[j].cardCanBePlacedOnStack(cards[index])) {
                    stacks[i].removeCards(k + 1);
                    for (int k2 = 0; k2 < k + 1; k2++) {
                        stacks[j].addCard(cards[index + k2]);
                    }
                    return true;
                }
            }
        }
    }
    return false;
}