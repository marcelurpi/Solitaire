
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
    std::string path = "res/cardsTilesheet.png";
    cardsTilesheetTexture = IMG_LoadTexture(renderer, path.c_str());
	if (cardsTilesheetTexture == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return;
	}
    font = TTF_OpenFont("res/KenneyHighSquare.ttf", 24);
}

void TextureManager::drawCard(int card, SDL_Rect* rect, bool transparent)
{
    SDL_Rect srcrect{(card % 14) * 64, (card / 14) * 64, 64, 64};
    int left = windowWidth / 2 - CARD_TEXTURE_SIZE / 2 + rect->x;
    int top = windowHeight / 2 - CARD_TEXTURE_SIZE / 2 + rect->y;
    SDL_Rect dstrect{left, top, CARD_TEXTURE_SIZE, CARD_TEXTURE_SIZE};
    if (transparent) {
        SDL_SetTextureAlphaMod(cardsTilesheetTexture, 128);
    }
    SDL_RenderCopy(renderer, cardsTilesheetTexture, &srcrect, &dstrect);
    if (transparent) {
        SDL_SetTextureAlphaMod(cardsTilesheetTexture, 255);
    }
}

void TextureManager::drawText(const std::string& text, int size, SDL_Point position)
{
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), { 255, 255, 255, 255 });
    int left = windowWidth / 2 - surface->w * size / 48 + position.x;
    int top = windowHeight / 2 - surface->h * size / 48 + position.y;
    SDL_Rect rect{ left, top, surface->w * size / 24, surface->h * size / 24 };
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
}