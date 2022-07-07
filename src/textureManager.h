
#ifndef HEADER_TEXTURE_MANAGER
#define HEADER_TEXTURE_MANAGER

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <map>

class TextureManager 
{
public:
    static TextureManager* Instance();
    void init(int windowWidth, int windowHeight, SDL_Renderer* renderer);
    void drawCard(int card, int x, int y, int w, int h);
private:
    static TextureManager* instance;
    int windowWidth;
    int windowHeight;
    SDL_Renderer* renderer;
    SDL_Texture* cardsTilesheetTexture;
};

#endif // HEADER_TEXTURE_MANAGER