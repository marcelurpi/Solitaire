
#include "game.h"
#include <stdio.h>
#include <iostream>

Game::Game() : renderer(nullptr)
{
    solitaire = new Solitaire();
}

Game::~Game() 
{
    SDL_DestroyWindow( window );
	SDL_Quit();
}

bool Game::init() 
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return false;
	}
    window = SDL_CreateWindow( "Solitaire", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN );
    if( window == NULL )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    TextureManager::Instance()->init(WINDOW_WIDTH, WINDOW_HEIGHT, renderer);
    return true;
}

bool Game::mainLoop() 
{
    SDL_Event event;
    while (SDL_PollEvent( &event ) != 0)
    {
        if (event.type == SDL_QUIT)
        {
            return false;
        }
        else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            solitaire->mouseDown(event.button.x - WINDOW_WIDTH / 2, event.button.y - WINDOW_HEIGHT / 2);
        }
        else if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT)
        {
            solitaire->mouseUp(event.button.x - WINDOW_WIDTH / 2, event.button.y - WINDOW_HEIGHT / 2);
        }
        else if (event.type == SDL_MOUSEMOTION)
        {
            solitaire->mouseDrag(event.motion.x - WINDOW_WIDTH / 2, event.motion.y - WINDOW_HEIGHT / 2);
        }
        else if (event.type == SDL_KEYDOWN)
        {
            solitaire->keyDown(event.key.keysym);
        }
        else if (event.type == SDL_KEYUP)
        {
            solitaire->keyUp(event.key.keysym);
        }
    }
    SDL_SetRenderDrawColor(renderer, 0x00, 0xAA, 0x55, 255);
    SDL_RenderClear(renderer);
    solitaire->draw();
    SDL_RenderPresent(renderer);

    return true;
}

SDL_Renderer* Game::getRenderer()
{
    return renderer;
}