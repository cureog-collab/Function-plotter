#ifndef MAIN_H
#define MAIN_H

#include "SDL2/SDL.h"
#include "camera_work.h"

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>

// CONSTANTS =============================================================================================
#define WINDOW_WIDTH 1500
#define WINDOW_HEGIHT 880
#define SCREEN_CENTERX (WINDOW_WIDTH * 0.5f)
#define SCREEN_CENTERY (WINDOW_HEGIHT *0.5f)
#define CUTOFF_HEIGHT 9000
// =======================================================================================================

// MEMORY ================================================================================================
// initate essential SDL-related stuff
bool initSDLGraphics(SDL_Window **window, SDL_Renderer **renderer, SDL_Texture **texture, SDL_Surface **surface);

// free everything
void destroyAllSDL(SDL_Window *window, SDL_Renderer *renderer, SDL_Texture *texture, SDL_Surface *surface);
// =======================================================================================================

// RENDER ================================================================================================
void drawGrid(SDL_Renderer *renderer, camera cam);
void drawFunction(SDL_FPoint *samplePoints, uint countSamplePts, SDL_Renderer *renderer, camera cam);
// =======================================================================================================

// HANDLE MATH ===========================================================================================
// output a list of sample points evaluated from the input function
bool scanFunction(const char *function, SDL_FPoint *samplePoints, const camera *cam);
// =======================================================================================================

// HANDLE LATEX ==========================================================================================
bool convertLatexToC(char *src, char *dst);
// =======================================================================================================

#endif