
#include "solitaire.h"

Solitaire::Solitaire()
{
    srand(time(0));
    resetBoard();
}

void Solitaire::resetBoard()
{
    movingCard = -1;
    cardStacks = std::vector<std::vector<int>>(NUM_COLUMNS);
    for (int i = 0; i < NUM_COLUMNS; i++) {
        cardStacks[i] = std::vector<int>(i + 1, 27);
        cardStacks[i][i] = (rand() % 4) * 14 + rand() % 13;
    }
}

void Solitaire::drawBoard()
{
    int leftX = -NUM_COLUMNS * 100 / 2;
    int topY = (-150 - NUM_COLUMNS * 20) / 2;
    TextureManager::Instance()->drawCard(27, leftX, topY, 128, 128);
    // TextureManager::Instance()->drawCard(0, leftX + 100, topY, 128, 128);
    TextureManager::Instance()->drawCard(13, leftX + 100 * 3, topY, 128, 128);
    TextureManager::Instance()->drawCard(13, leftX + 100 * 4, topY, 128, 128);
    TextureManager::Instance()->drawCard(13, leftX + 100 * 5, topY, 128, 128);
    TextureManager::Instance()->drawCard(13, leftX + 100 * 6, topY, 128, 128);
    for (int i = 0; i < NUM_COLUMNS; i++) {
        for (int j = 0; j < cardStacks[i].size(); j++) {
            TextureManager::Instance()->drawCard(cardStacks[i][j], leftX + 100 * i, topY + 150 + 20 * j, 128, 128);
        }
    }
    if (movingCard != -1) {
        TextureManager::Instance()->drawCard(movingCard, movingCardX, movingCardY, 128, 128);
    }
}

void Solitaire::mouseDown(int mouseX, int mouseY) 
{
    int leftX = -NUM_COLUMNS * 100 / 2;
    int topY = (-150 - NUM_COLUMNS * 20) / 2;
    for (int i = 0; i < NUM_COLUMNS; i++) {
        for (int j = cardStacks[i].size() - 1; j >= 0; j--) {
            int x = leftX + 100 * i;
            int y = topY + 150 + 20 * j;
            if (isMouseInside(mouseX, mouseY, leftX + 100 * i, topY + 150 + 20 * j, 84, 128)) {
                movingCard = cardStacks[i][j];
                movingCardX = leftX + 100 * i;
                movingCardY = topY + 150 + 20 * j;
                movingCardOffsetX = (leftX + 100 * i) - mouseX;
                movingCardOffsetY = (topY + 150 + 20 * j) - mouseY;
                return;
            }
        }
    }
}

void Solitaire::mouseUp(int mouseX, int mouseY)
{
    movingCard = -1;
}

void Solitaire::mouseDrag(int mouseX, int mouseY) 
{
    movingCardX = mouseX + movingCardOffsetX;
    movingCardY = mouseY + movingCardOffsetY;
}

bool Solitaire::isMouseInside(int mouseX, int mouseY, int x, int y, int w, int h)
{
    bool insideX = mouseX >= x - w / 2 && mouseX <= x + w / 2;
    return insideX && mouseY >= y - h / 2 && mouseY <= y + h / 2;
}