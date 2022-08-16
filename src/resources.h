
#ifndef RESOURCES
#define RESOURCES

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <map>

const int NUM_STACKS = 7;

const int CARD_WIDTH = 84;
const int CARD_HEIGHT = 120;
const int CARD_TEXTURE_SIZE = 128;

const int CARD_EMPTY = 13;
const int CARD_BACK = 27;

enum class Sound {
    CardPlace,
    CardShove,
    Winner,
};

class Resources 
{
public:
    static Resources* Instance();
    void init(int windowWidth, int windowHeight, SDL_Renderer* renderer);
    void drawCard(int card, SDL_Rect* rect, bool transparent);
    void drawText(const std::string& text, int size, SDL_Point position);
    void playSound(Sound sound);
private:
    static Resources* instance;
    int windowWidth;
    int windowHeight;
    SDL_Renderer* renderer;
    SDL_Texture* cardsTilesheetTexture;
    TTF_Font* font;
    Mix_Music* cardPlaceSound;
    Mix_Music* cardShoveSound;
    Mix_Music* winnerSound;
};

#endif // RESOURCES