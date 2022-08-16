
#ifndef HEADER_TEXTURE_MANAGER
#define HEADER_TEXTURE_MANAGER

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <map>

const int NUM_STACKS = 7;

const int CARD_WIDTH = 84;
const int CARD_HEIGHT = 120;
const int CARD_TEXTURE_SIZE = 128;

const int CARD_EMPTY = 13;
const int CARD_BACK = 27;

class TextureManager 
{
public:
    static TextureManager* Instance();
    void init(int windowWidth, int windowHeight, SDL_Renderer* renderer);
    void drawCard(int card, SDL_Rect* rect, bool transparent);
    void drawText(const std::string& text, int size, SDL_Point position);
private:
    static TextureManager* instance;
    int windowWidth;
    int windowHeight;
    SDL_Renderer* renderer;
    TTF_Font* font;
    SDL_Texture* cardsTilesheetTexture;
};

#endif // HEADER_TEXTURE_MANAGER