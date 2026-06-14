#include "../include/main.h"
#include "../include/tinyexpr.h"
#include "../include/camera_work.h"

#include <SDL2/SDL_rect.h>

bool scanFunction(const char *function, SDL_FPoint *samplePoints, const camera *cam)
{
    int error;
    double currGridX;
    te_variable replaceX = {
        "x",
        &currGridX,
        TE_VARIABLE,
        NULL
    };

    float invZoom = 1.0f / cam->zoom;
    SDL_FPoint gridOrigin = cameraMorphFPoint(cam, (SDL_FPoint){SCREEN_CENTERX, SCREEN_CENTERY});
    te_expr *proccessedExpr = te_compile(function, &replaceX, 1, &error);
    if (proccessedExpr == NULL)
    {
        printf("Error: failed to compile function!\n");
        return false;
    }

    for (float screenX = 0; screenX < WINDOW_WIDTH; ++screenX)
    {
        currGridX = (screenX - gridOrigin.x) * invZoom;
        samplePoints[(int)screenX].x = currGridX;
        samplePoints[(int)screenX].y = te_eval(proccessedExpr);
    }

    te_free(proccessedExpr);
    return true;
}