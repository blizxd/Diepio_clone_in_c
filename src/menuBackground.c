#include <menuBackground.h>
#include <game.h>
#include <helper.h>
#include <map.h>
#include <obstacle.h>

SDL_Rect decorativeCamera = {
    .x = 800,
    .y = 800,
    .w = SCREEN_WIDTH,
    .h = SCREEN_HEIGHT};

unsigned int msSinceUpdatedCameraDir;
Vec2d cameraDir;

int getRandomCameraDir()
{
  cameraDir.x = rand() % 10 - 5;
  cameraDir.y = rand() % 10 - 5;
  cameraDir = normalizeVec(cameraDir);

  return 0;
}

int initMainMenuBackground()
{
  getRandomCameraDir();
  return 0;
}

int updateMenuBackground()
{

  decorativeCamera.x += cameraDir.x * 5;
  decorativeCamera.y += cameraDir.y * 5;

  if (decorativeCamera.x < 0)
    decorativeCamera.x = 0;
  if (decorativeCamera.y < 0)
    decorativeCamera.y = 0;

  if (SDL_GetTicks() - msSinceUpdatedCameraDir > 1000)
  {
    msSinceUpdatedCameraDir = SDL_GetTicks();
    getRandomCameraDir();
  }

  return 0;
}

void renderMenuBackground()
{

  drawMap(renderer, &decorativeCamera);
  renderObstacleGroup(renderer, decorativeCamera);
}