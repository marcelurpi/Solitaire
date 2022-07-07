
#include "textureManager.h"

TextureManager* TextureManager::instance = nullptr;

TextureManager* TextureManager::Instance()
{
    if (instance == nullptr)
        instance = new TextureManager();
    return instance;
}

void TextureManager::init(int windowWidth, int windowHeight, SDL_Renderer* renderer)
{
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->renderer = renderer;
    std::string path = "resources/cardsTilesheet.png";
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
	if (texture == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return;
	}
    cardsTilesheetTexture = texture;
}

void TextureManager::drawCard(int card, int x, int y, int w, int h)
{
    SDL_Rect srcrect{(card % 14) * 64, (card / 14) * 64, 64, 64};
    int realX = windowWidth / 2 - w / 2 + x;
    int realY = windowHeight / 2 - h / 2 + y;
    SDL_Rect dstrect{ realX, realY, w, h};
    SDL_RenderCopy(renderer, cardsTilesheetTexture, &srcrect, &dstrect);
}