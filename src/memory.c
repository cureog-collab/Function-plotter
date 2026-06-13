#include "../include/main.h"

#include <SDL2/SDL_mutex.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>

// initate essential SDL-related stuff
bool initSDLGraphics(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **texture, SDL_Surface **surface)
{
    *window = SDL_CreateWindow("Function plotter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            WINDOW_WIDTH, WINDOW_HEGIHT, 0);
    if (*window == NULL)
    {
        printf("Error: failed to create window!\n");
        return false;
    }
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (*renderer == NULL)
    {
        printf("Error: failed to create renderer!\n");
        SDL_DestroyWindow(*window);
        return false;
    }
    *texture = SDL_CreateTexture(*renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 900, 600);
    if (*texture == NULL)
    {
        printf("Error: failed to create texture!\n");
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        return false;
    }
    *surface = SDL_GetWindowSurface(*window);
    if (*surface == NULL)
    {
        printf("Error: failed to create surface!\n");
        SDL_DestroyTexture(*texture);
        SDL_DestroyRenderer(*renderer);
        SDL_DestroyWindow(*window);
        return false;
    }
    
    return true;
}

// free everything
void destroyAllSDL(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture, SDL_Surface *surface)
{
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}