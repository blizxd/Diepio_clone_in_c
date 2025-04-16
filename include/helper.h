#ifndef HELPER_H
#define HELPER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <config.h>
typedef struct
{
  float x, y;

} Vec2d;

extern TTF_Font *fontSmall, *fontBig;

enum Textures
{
  PlayerImage,
  Pentagon,
  Tile,
  Triangle,
  BorderTile,
  Cube,
  Bullet,
  HealthbarBase,
  HealthbarFull,
  Scorebar,
  EnemyImage
};

enum XpEvent
{

  dealtDamageToEnemy,
  dealtDamageToObstacle

};

extern SDL_Texture *textureManager[MAX_TEXTURES];

void initTextures(SDL_Renderer *renderer);

double getAngle(SDL_Point p1, SDL_Point p2);

Vec2d normalizeVec(Vec2d vec);

SDL_Texture *loadTex(SDL_Renderer *renderer, char *path);

void renderSprite(SDL_Texture *tex, SDL_Rect pos, SDL_Rect camera, double angle, SDL_Renderer *renderer);

void renderSpriteWithSrc(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, SDL_Rect camera, double angle, SDL_Renderer *renderer);

void renderSpriteAbsolute(SDL_Texture *tex, SDL_Rect pos, double angle, SDL_Renderer *renderer);

void setCamera(SDL_Rect *camera, SDL_Rect playerRect);

int checkCollision(SDL_Rect a, SDL_Rect b);

void renderText(SDL_Point pos, SDL_Renderer *renderer, TTF_Font *font, char *text, SDL_Color color);

void initFonts();

void renderRect(SDL_Rect rect, SDL_Rect camera, SDL_Renderer *renderer);

int min(int a, int b);

int max(int a, int b);

#endif