#include <UI.h>
#include <enemy.h>
#include <game.h>
#include <helper.h>
#include <logtofile.h>
#include <mainMenu.h>
#include <map.h>
#include <obstacle.h>
#include <player.h>
#include <projectile.h>
#include <savegame.h>
#include <scoreboard.h>
#include <stdlib.h>
#include <string.h>

SDL_Window *window;
SDL_Renderer *renderer;

Player player;
ObstacleGroup obstacleGroup;

SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

enum Scenes currentScene;

char isRunning = 1, saveFileExists;

int msSinceSavegame;

int initGame()
{

    loadResources();

    saveFileExists = saveGamefileExists();

    msSinceSavegame = SDL_GetTicks();

    return 0;
}

int loadResources()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {

        printf("Error initializing SDL: %s\n", SDL_GetError());

        return 1;
    }
    printf("SDL successfully initialized!\n");

    if (TTF_Init() != 0)
    {
        printf("error initializing SDL_ttf");
    }
    printf("SDL_ttf successfully initalized \n");

    SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);
    SDL_SetWindowTitle(window, "My game");

    initTextures(renderer);

    initMap(renderer);

    srand(time(NULL));

    initFonts();
    initMainMenuBackground();

    RestartGameLogic();

    currentScene = MAIN_MENU;

    return 0;
}

int RestartGameLogic()
{

    memset(&player, 0, sizeof(player));
    memset(&obstacleGroup, 0, sizeof(obstacleGroup));
    memset(&EG, 0, sizeof(EG));
    EG.enemies = NULL;

    obstacleGroup.activeCount = 0;
    createPlayer(&player, renderer, (SDL_Point){0, 0});
    createRandomObstacles(renderer, MAX_OBSTACLES);

    return 0;
}

void createNewGame()
{
    msSinceSavegame = SDL_GetTicks();

    RestartGameLogic();
    currentScene = GAME;
}

void loadGameFromFile()
{
    msSinceSavegame = SDL_GetTicks();

    getGamedataFromFile(&player, &obstacleGroup);
    currentScene = GAME;
}

int handleUserInput()
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {

        if (e.type == SDL_QUIT)
        {
            isRunning = 0;
        }

        else if (currentScene == GAME)
        {
            handlePlayerInput(&player, e);
            handleUIInput(e, &player);
        }
        else if (currentScene == MAIN_MENU)
        {
            handleMenuInput(e);
        }
        else if (currentScene == SCOREBOARD)
        {
            handleScoreBoardInput(e);
        }
    }

    return 0;
}

int gameUpdate()
{

    if (SDL_GetTicks() - msSinceSavegame > 5000 && AUTOSAVE)
    {
        saveGamedataToFile(&player, &obstacleGroup);
        msSinceSavegame = SDL_GetTicks();
    }

    updatePlayer(&player, camera, renderer);

    spawnRandomObstacle(renderer);

    updateProjectiles(&player.projGroup);

    updateEnemyGroup(&player);

    setCamera(&camera, player.pos);

    return 0;
}

int gameRender()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    drawMap(renderer, &camera);

    renderPlayer(&player, camera, renderer);

    renderObstacleGroup(renderer, camera);

    renderProjectiles(&player.projGroup, renderer);

    renderEnemyGroup();

    displayScore(renderer, &player);

    displayAvailableUpgrades(renderer, &player);

    displayMessage();

    displayKillCount(&player);

    SDL_RenderPresent(renderer);

    return 0;
}

void switchToMainMenu()
{
    saveFileExists = saveGamefileExists();
    initMainMenuBackground();
    RestartGameLogic();
    currentScene = MAIN_MENU;
}

void shutdownGame()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int runGame()
{
    handleUserInput();

    if (currentScene == MAIN_MENU)
    {
        updateMenuBackground();
        renderMenu();
    }
    else if (currentScene == SCOREBOARD)
    {
        updateMenuBackground();
        displayScoreboard();
    }
    else if (currentScene == GAME)
    {
        gameUpdate();
        gameRender();
    }
    return 0;
}