#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <SDL2/SDL.h>
#include <config.h>
#include <helper.h>
#include <math.h>
#include <obstacle.h>

typedef struct Player Player;
typedef struct
{

    SDL_Rect pos, collider;

    SDL_Texture *texture;

    Vec2d vel;

    int speed, maxRange, distance;
    char isActive;
    int damage;

} Projectile;

typedef struct
{
    Projectile projectiles[MAX_PROJECTILES];
    int activeCount;
    Player *owner;
} ProjectileGroup;
void updateProjectile(ProjectileGroup *pg, Projectile *proj);

void updateProjectiles(ProjectileGroup *pg);

void renderProjectile(SDL_Renderer *renderer, Projectile *proj);

void renderProjectiles(ProjectileGroup *pg, SDL_Renderer *renderer);

int spawnProjectile(ProjectileGroup *pg, SDL_Renderer *renderer, SDL_Point startPos,
                    Vec2d startVel);

void checkProjectileObstacleCollision(ProjectileGroup *pg, Projectile *proj);

void checkProjectileEnemyCollision(Projectile *proj, ProjectileGroup *pg);
#endif