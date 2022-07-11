
#ifndef HEADER_TEXTURE_MANAGER
#define HEADER_TEXTURE_MANAGER

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <map>

const int CARD_TEXTURE_SIZE = 128;

class TextureManager 
{
public:
    static TextureManager* Instance();
    void init(int windowWidth, int windowHeight, SDL_Renderer* renderer);
    void drawCard(int card, SDL_Rect* rect);
private:
    static TextureManager* instance;
    int windowWidth;
    int windowHeight;
    SDL_Renderer* renderer;
    SDL_Texture* cardsTilesheetTexture;
};

#endif // HEADER_TEXTURE_MANAGER