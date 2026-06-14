#include "../include/main.h"

#include <SDL2/SDL_render.h>
#include <math.h>

// draw grid and its principle axises
void drawGrid(SDL_Renderer *renderer, camera cam)
{
    if (renderer == NULL)
    {
        printf("Error: NULL renderer passed to drawGrid()!\n");
        return;
    }

    // color for the main axises
    SDL_SetRenderDrawColor(renderer, 227, 76, 76, 255);

    // find the position of the grid center on screen
    float screenX = (SCREEN_CENTERX * cam.zoom) + cam.camPos.x;
    float screenY = (SCREEN_CENTERY * cam.zoom) + cam.camPos.y;

    // calculation to do the "elastic grid" effect like in Desmos when zooming in/ out
    float mathWidth = WINDOW_WIDTH / (cam.zoom);
    float rawMagnitude = log10f(mathWidth) - 1.0f;
    float remainderMagnitude = rawMagnitude - (int)rawMagnitude;
    float minorStep = powf(10.0f, (int)rawMagnitude); // step for the minor grid
    float majorStep = minorStep * 10.0f; // step for the major grid

    // translate to screen step
    float screenMinorStep = minorStep * cam.zoom;
    float screenMajorStep = majorStep * cam.zoom;

    // draw X-axis (3-pixel thick)
    SDL_RenderDrawLineF(renderer, 0.0f, screenY, WINDOW_WIDTH, screenY);
    SDL_RenderDrawLineF(renderer, 0.0f, screenY + 1.0f, WINDOW_WIDTH, screenY + 1.0f);
    SDL_RenderDrawLineF(renderer, 0.0f, screenY - 1.0f, WINDOW_WIDTH, screenY - 1.0f);

    // draw Y-axis (3-pixel thick)
    SDL_RenderDrawLineF(renderer, screenX, 0.0f, screenX, WINDOW_HEGIHT);
    SDL_RenderDrawLineF(renderer, screenX + 1.0f, 0.0f, screenX + 1.0f, WINDOW_HEGIHT);
    SDL_RenderDrawLineF(renderer, screenX - 1.0f, 0.0f, screenX - 1.0f,  WINDOW_HEGIHT);

    // draw XY grid
    // draw minor grid (fades out)
    float startGridX = fmodf(screenX, screenMinorStep);
    float startGridY = fmodf(screenY, screenMinorStep);
    SDL_SetRenderDrawColor(renderer, 201, 201, 201, (Uint8)(230.0f * (1.0f - remainderMagnitude)));
    for (float row = startGridY; row < WINDOW_HEGIHT; row += screenMinorStep)
    {
        SDL_RenderDrawLineF(renderer, 0, row, WINDOW_WIDTH, row);
    }
    for (float col = startGridX; col < WINDOW_WIDTH; col += screenMinorStep)
    {
        SDL_RenderDrawLineF(renderer, col, 0, col, WINDOW_HEGIHT);
    }

    // draw major grid
    float startGridMajorX = fmodf(screenX, screenMajorStep);
    float startGridMajorY = fmodf(screenY, screenMajorStep);
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 230);
    for (float row = startGridMajorY; row < WINDOW_HEGIHT; row += screenMajorStep)
    {
        SDL_RenderDrawLineF(renderer, 0, row, WINDOW_WIDTH, row);
    }
    for (float col = startGridMajorX; col < WINDOW_WIDTH; col += screenMajorStep)
    {
        SDL_RenderDrawLineF(renderer, col, 0, col, WINDOW_HEGIHT);
    }
}

void drawFunction(SDL_FPoint *samplePoints, uint countSamplePts, SDL_Renderer *renderer, camera cam)
{
    float screenX = (SCREEN_CENTERX * cam.zoom) + cam.camPos.x;
    float screenY = (SCREEN_CENTERY * cam.zoom) + cam.camPos.y;

    int segmentStart = -1; // not plotting

    for (uint i = 0; i < countSamplePts; ++i)
    {
        // check if y(x) is a valid number to plot
        float mathY = samplePoints[i].y;
        bool isValid = !isnanf(mathY) && !isinff(mathY);

        samplePoints[i].x = samplePoints[i].x * cam.zoom + screenX;
        samplePoints[i].y = screenY- (samplePoints[i].y * cam.zoom);

        if (samplePoints[i].y >= CUTOFF_HEIGHT || samplePoints[i].y < -CUTOFF_HEIGHT)
        {
            isValid = false;
        }

        if (isValid)
        {
            if (segmentStart == -1)
            {
                segmentStart = i;
            }
        }
        else
        {
            if (segmentStart != -1)
            {
                int currSegmentPts = i - segmentStart;
                if (currSegmentPts > 1)
                {
                    SDL_RenderDrawLinesF(renderer, &samplePoints[segmentStart], currSegmentPts);
                }
                segmentStart = -1;
            }        
        }
    }

    if (segmentStart != -1)
    {
        int currSegmentPts = countSamplePts - segmentStart;
        if (currSegmentPts > 1)
        {
            SDL_RenderDrawLinesF(renderer, &samplePoints[segmentStart], currSegmentPts);
        }
    }
}