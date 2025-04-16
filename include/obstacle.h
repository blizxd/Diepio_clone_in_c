#ifndef OBSTACLE_H
#define OBSTACLE_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <helper.h>
#include <game.h>
#include <config.h>
#include <healthbar.h>

typedef struct
{
  SDL_Texture *texture;

  SDL_Rect pos;

  char type;

  double angle;
  int hp;
  int maxHp;
  int inactive;

  SDL_Rect collider;

} Obstacle;

typedef struct
{
  Obstacle obstacles[MAX_OBSTACLES];
  int activeCount;
  unsigned int msSincePrevSpawn;
} ObstacleGroup;

extern ObstacleGroup obstacleGroup;

void serializeObsGroup(ObstacleGroup *obsg, char *buffer);

void deserializeObsGroup(char *buffer, ObstacleGroup *obsg, SDL_Renderer *renderer);

void initObstacle(Obstacle *obs, SDL_Renderer *renderer, char type, SDL_Point pos);

int createObstacle(Obstacle *obs, SDL_Renderer *renderer, char type, SDL_Point pos);

void renderObstacle(Obstacle *obs, SDL_Rect camera, SDL_Renderer *renderer);

int checkCollisionBetweenObs(Obstacle *obs);

void createRandomObstacles(SDL_Renderer *renderer, int obsCount);

void renderObstacleGroup(SDL_Renderer *renderer, SDL_Rect camera);

void destroyObstacle(Obstacle *obs);

int dealDmgToObstacleAndGetXp(Obstacle *obs, int damage);
void spawnRandomObstacle(SDL_Renderer *renderer);

#endif