#include <SDL2/SDL.h>
#include <game.h>
#include <logtofile.h>
#include <stdio.h>

#define DEBUG 0

int main(int argc, char *argv[])
{

    unsigned long long startTicks;
    float frameTime, targetFtime;

    targetFtime = (float)1000 / 60;

    initGame();
    logGameStartToFile();

    while (isRunning)
    {

        startTicks = SDL_GetPerformanceCounter();

        runGame();

        frameTime = (SDL_GetPerformanceCounter() - startTicks) /
                    (float)SDL_GetPerformanceFrequency() * 1000.0f;

        if (targetFtime > frameTime)
            SDL_Delay(targetFtime - frameTime);
    }
    shutdownGame();
    logGameEndToFile();
    return 0;
}
