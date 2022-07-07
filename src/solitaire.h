
#ifndef HEADER_SOLITAIRE
#define HEADER_SOLITAIRE

#include <vector>
#include "textureManager.h"

const int NUM_COLUMNS = 7;

class Solitaire 
{
public:
    Solitaire();
    void resetBoard();
    void drawBoard();
    void mouseDown(int mouseX, int mouseY);
    void mouseUp(int mouseX, int mouseY);
    void mouseDrag(int mouseX, int mouseY);
private:
    bool isMouseInside(int mouseX, int mouseY, int x, int y, int w, int h);

    int movingCard;
    int movingCardOffsetX;
    int movingCardOffsetY;
    int movingCardX;
    int movingCardY;
    std::vector<std::vector<int>> cardStacks;
};

#endif // HEADER_SOLITAIRE