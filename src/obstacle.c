#include <obstacle.h>

void serializeObsGroup(ObstacleGroup *obsg, char *buffer)
{

    memcpy(buffer, &obsg->activeCount, sizeof(int));
    buffer += sizeof(int);
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        memcpy(buffer, &obsg->obstacles[i].pos.x, sizeof(int));
        buffer += sizeof(int);
        memcpy(buffer, &obsg->obstacles[i].pos.y, sizeof(int));
        buffer += sizeof(int);
        memcpy(buffer, &obsg->obstacles[i].hp, sizeof(int));
        buffer += sizeof(int);
        memcpy(buffer, &obsg->obstacles[i].type, 1);
        buffer += 1;
        memcpy(buffer, &obsg->obstacles[i].angle, sizeof(double));
        buffer += sizeof(double);
        memcpy(buffer, &obsg->obstacles[i].inactive, sizeof(int));
        buffer += sizeof(int);
    }
}

void deserializeObsGroup(char *buffer, ObstacleGroup *obsg, SDL_Renderer *renderer)
{
    memcpy((&obsg->activeCount), buffer, sizeof(int));
    buffer += sizeof(int);

    SDL_Point pos;
    char type;
    int hp, inactive;
    double angle;

    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        memcpy(&pos.x, buffer, sizeof(int));
        buffer += sizeof(int);
        memcpy(&pos.y, buffer, sizeof(int));
        buffer += sizeof(int);
        memcpy(&hp, buffer, sizeof(int));
        buffer += sizeof(int);
        memcpy(&type, buffer, 1);
        buffer += 1;
        memcpy(&angle, buffer, sizeof(double));
        buffer += sizeof(double);
        memcpy(buffer, &inactive, sizeof(int));
        buffer += sizeof(int);

        pos.x -= SCREEN_WIDTH / 2;
        pos.y -= SCREEN_HEIGHT / 2;

        initObstacle(&obsg->obstacles[i], renderer, type, pos);
        obsg->obstacles[i].angle = angle;
        obsg->obstacles[i].hp = hp;
        obsg->obstacles[i].inactive = inactive;
    }
}

void initObstacle(Obstacle *obs, SDL_Renderer *renderer, char type, SDL_Point pos)
{
    enum Textures texture;

    switch (type)
    {
    case 0:
        texture = Cube;
        obs->pos.w = 80;
        obs->pos.h = 80;
        obs->maxHp = 50;
        break;
    case 1:
        texture = Triangle;
        obs->pos.w = 80;
        obs->pos.h = 80;
        obs->maxHp = 150;
        break;
    case 2:
        texture = Pentagon;
        obs->pos.w = 160;
        obs->pos.h = 160;
        obs->maxHp = 400;
        break;
    }

    obs->type = type;

    obs->hp = obs->maxHp;

    obs->inactive = 0;

    obs->pos.x = pos.x + SCREEN_WIDTH / 2;
    obs->pos.y = pos.y + SCREEN_HEIGHT / 2;

    obs->collider = obs->pos;
    obs->texture = textureManager[texture];
}

int createObstacle(Obstacle *obs, SDL_Renderer *renderer, char type, SDL_Point pos)
{

    if (obstacleGroup.activeCount == MAX_OBSTACLES)
        return 0;

    initObstacle(obs, renderer, type, pos);

    if (checkCollisionBetweenObs(obs) || checkCollision(camera, obs->collider))
    {
        return 0;
    }

    obstacleGroup.activeCount++;

    return 1;
}

void renderObstacle(Obstacle *obs, SDL_Rect camera, SDL_Renderer *renderer)
{
    renderSprite(obs->texture, obs->pos, camera, obs->angle, renderer);
    if (SHOW_BOX)
        renderRect(obs->collider, camera, renderer);
    if (obs->hp != obs->maxHp)
        renderHealthBar(renderer, (float)obs->hp / obs->maxHp,
                        (SDL_Point){obs->pos.x + obs->pos.w / 2, obs->pos.y}, camera);
}

int checkCollisionBetweenObs(Obstacle *obs)
{

    for (int i = 0; i < obstacleGroup.activeCount; i++)
    {
        if (checkCollision(obs->collider, obstacleGroup.obstacles[i].collider))
            return 1;
    }

    return 0;
}

void createRandomObstacles(SDL_Renderer *renderer, int obsCount)
{
    int createdObs = 0;
    while (obstacleGroup.activeCount < MAX_OBSTACLES && createdObs < obsCount)
    {
        int x_pos = rand() % (LEVEL_WIDTH - 200);
        int y_pos = rand() % (LEVEL_HEIGHT - 200);
        double rot = rand() % 360;
        int type = rand() % 3;

        obstacleGroup.obstacles[obstacleGroup.activeCount].angle = rot;
        if (createObstacle(&obstacleGroup.obstacles[obstacleGroup.activeCount], renderer, type,
                           (SDL_Point){x_pos, y_pos}))
        {
            createdObs++;
        }
    }
}

void renderObstacleGroup(SDL_Renderer *renderer, SDL_Rect camera)
{
    for (int i = 0; i < obstacleGroup.activeCount; i++)
    {
        renderObstacle(&obstacleGroup.obstacles[i], camera, renderer);
    }
}

void destroyObstacle(Obstacle *obs)
{
    *obs = obstacleGroup.obstacles[obstacleGroup.activeCount - 1];
    obstacleGroup.activeCount--;
}

int dealDmgToObstacleAndGetXp(Obstacle *obs, int damage)
{
    if ((obs->hp -= damage) <= 0)
    {
        obs->hp = 0;
        obs->inactive = 1;

        int xp;
        switch (obs->type)
        {
        case 0:
            xp = 50;
            break;
        case 1:
            xp = 200;
            break;
        case 2:
            xp = 1000;
            break;
        }

        destroyObstacle(obs);
        return xp;
    }
    return 0;
}

void spawnRandomObstacle(SDL_Renderer *renderer)
{

    unsigned int currentTicks = SDL_GetTicks();

    if (currentTicks - obstacleGroup.msSincePrevSpawn > OBSTACLE_RESPAWN_TIME)
    {
        createRandomObstacles(renderer, 1);
        obstacleGroup.msSincePrevSpawn = currentTicks;
    }
}
