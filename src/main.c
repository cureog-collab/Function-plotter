#include "../include/main.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    // terminal-based function input
    char inputFunction[2048];
    if (argc > 2)
    {
        printf("Usage: %s [input-function]\n", argv[0]);
        return 1;
    }
    else if (argc == 2)
    {
        if (!convertLatexToC(argv[1], inputFunction))
        {
            printf("Error: failed to read input function!\n");
            return 1;
        }
    }

    // initiate stuff
    SDL_Window *mainWindow = NULL;
    SDL_Renderer *mainRenderer = NULL;
    SDL_Texture *mainTexture = NULL;
    SDL_Surface *mainSurface = NULL;
    if (!initSDLGraphics(&mainWindow, &mainRenderer, &mainTexture, &mainSurface))
    {
        printf("Error: failed to initiate SDL!\n");
        return 0;
    }
    camera mainCam;

    // setting up initial settings
    bool isOn = true;
    bool needReDraw = true;
    SDL_Event mainEvent;
    SDL_Point orgin = {0, 0};
    SDL_FPoint *samplePoints = malloc(WINDOW_WIDTH * sizeof(SDL_FPoint));
    resetCamera(&mainCam, orgin);

    //main program loop
    while (isOn)
    {
        while (SDL_PollEvent(&mainEvent))
        {
            needReDraw = true;
            switch (mainEvent.type)
            {
                case SDL_QUIT:
                {
                    isOn = false;
                    break;
                }

                // reset camera settings when pressing 'SPACE'
                case SDL_KEYDOWN:
                {
                    if (mainEvent.key.keysym.sym == SDLK_SPACE)
                    {
                        resetCamera(&mainCam, orgin);
                    }
                    break;
                }
            }

            updateViewport(&mainCam, &mainEvent);
            // TODO
            // user interactive
        }

        if (needReDraw)
        {
            SDL_SetRenderDrawColor(mainRenderer, 252, 251, 237, 1);
            SDL_RenderClear(mainRenderer);

            // draw principle axises
            SDL_SetRenderDrawBlendMode(mainRenderer, SDL_BLENDMODE_BLEND);
            drawGrid(mainRenderer, mainCam);
            if (!scanFunction(inputFunction, samplePoints, &mainCam))
            {
                return 1;
            }

            SDL_SetRenderDrawColor(mainRenderer, 45, 112, 179, 255);
            drawFunction(samplePoints, WINDOW_WIDTH, mainRenderer, mainCam);
            // basically: desmos on a budget

            SDL_RenderPresent(mainRenderer);
            needReDraw = false;
        }
        SDL_Delay(16);
    }

    free(samplePoints);
    destroyAllSDL(mainWindow, mainRenderer, mainTexture, mainSurface);
    SDL_Quit();
}