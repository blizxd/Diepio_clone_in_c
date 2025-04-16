#include <UI.h>
#include <mainMenu.h>
#include <map.h>
#include <savegame.h>
#include <scoreboard.h>
#include <stdlib.h>

static SDL_Point mousePos;

int handleMenuInput(SDL_Event e)
{
    if (e.type == SDL_MOUSEMOTION)
    {
        SDL_GetMouseState(&mousePos.x, &mousePos.y);
    }
    else if (e.type == SDL_KEYDOWN)
    {
        if (e.key.keysym.sym == SDLK_1 && saveFileExists)
            loadGameFromFile();
        if (e.key.keysym.sym == SDLK_2)
            createNewGame();
        if (e.key.keysym.sym == SDLK_3)
            switchToScoreboard();
    }
    return 0;
}

int renderMenu()
{
    renderMenuBackground();
    SDL_Color color = {0, 0, 0};

    SDL_Point pos = (SDL_Point){.x = SCREEN_WIDTH / 2, .y = SCREEN_HEIGHT / 2};

    char *text = "Diep.io Game Clone\nAuthor : Nikita M";

    renderText(pos, renderer, fontBig, text, color);

    if (saveFileExists)
        text = "Press 1 to load from save\nPress 2 to start new game\nPress 3 to view player score";
    else
        text = "No save game found\nPress 2 to start new game\nPress 3 to view player score";

    pos.y += 200;

    renderText(pos, renderer, fontBig, text, color);

    SDL_RenderPresent(renderer);
    return 0;
}