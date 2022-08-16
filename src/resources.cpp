
#include "Resources.h"

Resources* Resources::instance = nullptr;

Resources* Resources::Instance()
{
    if (instance == nullptr)
        instance = new Resources();
    return instance;
}

void Resources::init(int windowWidth, int windowHeight, SDL_Renderer* renderer)
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
    path = "res/kenneyHighSquare.ttf";
    font = TTF_OpenFont(path.c_str(), 24);
    if (font == NULL)
	{
		printf("Unable to load font %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return;
	}
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 512) < 0) {
        printf("Unable to setup audio! SDL Error: %s\n", SDL_GetError());
        return;
    }
    path = "res/cardPlace.ogg";
    cardPlaceSound = Mix_LoadMUS(path.c_str());
    if (cardPlaceSound == NULL)
	{
		printf("Unable to load sound %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return;
	}
    path = "res/cardShove.ogg";
    cardShoveSound = Mix_LoadMUS(path.c_str());
    if (cardShoveSound == NULL)
	{
		printf("Unable to load sound %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return;
	}
    path = "res/winner.ogg";
    winnerSound = Mix_LoadMUS(path.c_str());
    if (winnerSound == NULL)
	{
		printf("Unable to load sound %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        return;
	}
}

void Resources::drawCard(int card, SDL_Rect* rect, bool transparent)
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

void Resources::drawText(const std::string& text, int size, SDL_Point position)
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

void Resources::playSound(Sound sound)
{
    if (sound == Sound::CardPlace) {
        Mix_PlayMusic(cardPlaceSound, 0);
    } else if (sound == Sound::CardShove) {
        Mix_PlayMusic(cardShoveSound, 0);
    } else if (sound == Sound::Winner) {
        Mix_PlayMusic(winnerSound, 0);
    }
}