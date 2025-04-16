#include <enemy.h>
#include <player.h>
#include <projectile.h>
void updateProjectile(ProjectileGroup *pg, Projectile *proj)
{

    proj->pos.x += proj->vel.x;
    proj->collider.x += proj->vel.x;

    if ((proj->pos.x < SCREEN_WIDTH / 2) ||
        (proj->pos.x + proj->pos.w > LEVEL_WIDTH + SCREEN_WIDTH / 2))
    {
        proj->isActive = 0;
        pg->activeCount--;
        return;
    }

    proj->pos.y += proj->vel.y;
    proj->collider.y += proj->vel.y;

    if ((proj->pos.y < SCREEN_HEIGHT / 2) ||
        (proj->pos.y + proj->pos.h > LEVEL_HEIGHT + SCREEN_HEIGHT / 2))
    {
        proj->isActive = 0;
        pg->activeCount--;
        return;
    }

    proj->distance += sqrt(pow(proj->vel.x, 2) + pow(proj->vel.y, 2));

    if (proj->distance > proj->maxRange)
    {
        proj->isActive = 0;
        pg->activeCount--;
        return;
    }

    checkProjectileObstacleCollision(pg, proj);
    checkProjectileEnemyCollision(proj, pg);
};

int spawnProjectile(ProjectileGroup *pg, SDL_Renderer *renderer, SDL_Point startPos, Vec2d startVel)
{
    if (pg->activeCount == MAX_PROJECTILES)
        return 0;

    Projectile *proj = NULL;

    for (int i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!pg->projectiles[i].isActive)
            proj = &pg->projectiles[i];
    }

    if (proj == NULL)
        return 0;

    proj->isActive = 1;
    proj->damage = 50 * pg->owner->bulletDamageMult;

    proj->distance = 0;
    proj->maxRange = 1500;

    proj->pos.x = startPos.x - proj->pos.w / 2;
    proj->pos.y = startPos.y - proj->pos.h / 2;

    proj->pos.w = 40;
    proj->pos.h = 40;

    proj->collider = proj->pos;

    proj->speed = PROJ_SPEED;
    proj->vel = startVel;

    proj->texture = textureManager[Bullet];

    pg->activeCount++;

    return 1;
}

void renderProjectile(SDL_Renderer *renderer, Projectile *proj)
{
    renderSprite(proj->texture, proj->pos, camera, 0, renderer);
    if (SHOW_BOX)
        renderRect(proj->collider, camera, renderer);
}

void updateProjectiles(ProjectileGroup *pg)
{

    for (int i = 0; i < MAX_PROJECTILES; i++)
    {
        if (pg->projectiles[i].isActive)
            updateProjectile(pg, &pg->projectiles[i]);
    }
}

void renderProjectiles(ProjectileGroup *pg, SDL_Renderer *renderer)
{
    for (int i = 0; i < MAX_PROJECTILES; i++)
    {
        if (pg->projectiles[i].isActive)
            renderProjectile(renderer, &pg->projectiles[i]);
    }
}

void checkProjectileObstacleCollision(ProjectileGroup *pg, Projectile *proj)
{
    for (int i = 0; i < obstacleGroup.activeCount; i++)
    {
        if (checkCollision(proj->collider, obstacleGroup.obstacles[i].collider))
        {
            updateCurrentScore(pg->owner,
                               dealDmgToObstacleAndGetXp(&obstacleGroup.obstacles[i], proj->damage),
                               dealtDamageToObstacle);
            proj->isActive = 0;
            pg->activeCount--;
            return;
        }
    }
}

void checkProjectileEnemyCollision(Projectile *proj, ProjectileGroup *pg)
{
    for (int j = 0; j < EG.activeCount; j++)
    {
        if (EG.enemies[j].isAlive && checkCollision(proj->collider, EG.enemies[j].collider))
        {
            updateCurrentScore(pg->owner, dealDamageToEnemyAndGetXp(j, proj->damage),
                               dealtDamageToEnemy);
            proj->isActive = 0;
            pg->activeCount--;
            return;
        }
    }
}
