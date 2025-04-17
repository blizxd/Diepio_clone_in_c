#include <UI.h>
#include <string.h>

char message[50] = "";
int msSinceDisplayingMessage = 0;

void displayScore(SDL_Renderer *renderer, Player *player)
{
    SDL_Color color = {255, 255, 255};
    char textStr[50];
    snprintf(textStr, 50, "Score : %d", player->currentScore);
    SDL_Rect scorebarPos =
        (SDL_Rect){.x = SCREEN_WIDTH / 2 - 125, .y = SCREEN_HEIGHT - 100, .w = 250, .h = 60};
    SDL_Point textPos = {.x = SCREEN_WIDTH / 2, .y = SCREEN_HEIGHT - 100 + 30};

    renderSpriteAbsolute(textureManager[Scorebar], scorebarPos, 0, renderer);
    renderText(textPos, renderer, fontSmall, textStr, color);
}

void displayKillCount(Player *player)
{

    SDL_Point textPos = {.x = 150, .y = SCREEN_HEIGHT / 2 + 200};

    char text[50];

    sprintf(text, "Enemies destroyed: %d", player->killCount);

    SDL_Color color = {0, 0, 0};

    renderText(textPos, renderer, fontSmall, text, color);
}

void setMessage(char *str)
{

    strcpy(message, str);

    msSinceDisplayingMessage = SDL_GetTicks();
}

void displayMessage()
{

    if (SDL_GetTicks() - msSinceDisplayingMessage > 2000)
        return;

    SDL_Color color = {0, 0, 0};
    SDL_Point textPos = {.x = SCREEN_WIDTH / 2 - 400, .y = 50};
    renderText(textPos, renderer, fontSmall, message, color);
}

void displayAvailableUpgrades(SDL_Renderer *renderer, Player *player)
{

    SDL_Point textPos = {.x = 200, .y = SCREEN_HEIGHT / 2 + 300};

    char text[100];

    sprintf(
        text,
        "Available upgrades : %d\n[1]speed: %d\n[2]bullet damage: %d\n[3]health: %d\n[4]reload :%d",
        player->availableUpgrades, player->speedLevel, player->bulletDamageLevel,
        player->healthLevel, player->reloadSpeedLevel);

    SDL_Color color = {0, 0, 0};

    renderText(textPos, renderer, fontSmall, text, color);
}

void handleUIInput(SDL_Event e, Player *player)
{

    if (e.type == SDL_KEYDOWN)
    {
        if (e.key.keysym.sym == SDLK_1)
            choosePlayerUpgrade(player, speedUpgrade);
        else if (e.key.keysym.sym == SDLK_2)
            choosePlayerUpgrade(player, bulletDamageUpgrade);
        else if (e.key.keysym.sym == SDLK_3)
            choosePlayerUpgrade(player, maxHpUpgrade);
        else if (e.key.keysym.sym == SDLK_4)
            choosePlayerUpgrade(player, reloadUpgrade);
        else if (e.key.keysym.sym == SDLK_F5)
        {
            setMessage("Saving game");
            saveGamedataToFile(player, &obstacleGroup);
        }
        else if (e.key.keysym.sym == SDLK_ESCAPE)
        {
            switchToMainMenu();
        }
    }
}
