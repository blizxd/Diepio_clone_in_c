#include <game.h>
#include <helper.h>
#include <math.h>
#include <string.h>
SDL_Texture *textureManager[MAX_TEXTURES];
TTF_Font *fontSmall, *fontBig;

void initTextures(SDL_Renderer *renderer)
{

    textureManager[PlayerImage] = loadTex(renderer, "./assets/PlayerImage.png");
    textureManager[Pentagon] = loadTex(renderer, "./assets/Pentagon.png");
    textureManager[Tile] = loadTex(renderer, "./assets/Tile.png");
    textureManager[Triangle] = loadTex(renderer, "./assets/Triangle.png");
    textureManager[BorderTile] = loadTex(renderer, "./assets/BorderTile.png");
    textureManager[Cube] = loadTex(renderer, "./assets/Cube.png");
    textureManager[Bullet] = loadTex(renderer, "./assets/Bullet.png");
    textureManager[HealthbarBase] = loadTex(renderer, "./assets/HealthbarBase.png");
    textureManager[HealthbarFull] = loadTex(renderer, "./assets/HealthbarFull.png");
    textureManager[Scorebar] = loadTex(renderer, "./assets/Scorebar.png");
    textureManager[EnemyImage] = loadTex(renderer, "./assets/Enemy.png");
}

double getAngle(SDL_Point p1, SDL_Point p2)
{

    double newAngle = (atan2(p1.y - p2.y, p1.x - p2.x)) * (180 / M_PI) + 90;

    return newAngle;
}

Vec2d normalizeVec(Vec2d vec)
{
    if (vec.x == 0 && vec.y == 0)
        return vec;

    Vec2d output;

    float vecLength = sqrt(pow(vec.x, 2) + pow(vec.y, 2));
    output.x = vec.x / vecLength;
    output.y = vec.y / vecLength;

    return output;
}

SDL_Texture *loadTex(SDL_Renderer *renderer, char *path)
{

    SDL_Surface *load = IMG_Load(path);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, load);
    SDL_FreeSurface(load);
    return texture;
}

void setCamera(SDL_Rect *camera, SDL_Rect playerRect)
{

    camera->w = SCREEN_WIDTH;
    camera->h = SCREEN_HEIGHT;
    camera->x = (playerRect.x + playerRect.w / 2) - SCREEN_WIDTH / 2;
    camera->y = (playerRect.y + playerRect.h / 2) - SCREEN_HEIGHT / 2;

    if (camera->x < 0)
    {
        camera->x = 0;
    }
    if (camera->y < 0)
    {
        camera->y = 0;
    }
    if (camera->x > TOTAL_WIDTH - camera->w)
    {
        camera->x = TOTAL_WIDTH - camera->w;
    }
    if (camera->y > TOTAL_HEIGHT - camera->h)
    {
        camera->y = TOTAL_HEIGHT - camera->h;
    }
}

void renderSprite(SDL_Texture *tex, SDL_Rect pos, SDL_Rect camera, double angle,
                  SDL_Renderer *renderer)
{

    SDL_Rect dest;
    dest.x = pos.x - camera.x;
    dest.y = pos.y - camera.y;
    dest.w = pos.w;
    dest.h = pos.h;

    SDL_RenderCopyEx(renderer, tex, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
}

void renderSpriteWithSrc(SDL_Texture *tex, SDL_Rect src, SDL_Rect pos, SDL_Rect camera,
                         double angle, SDL_Renderer *renderer)
{
    SDL_Rect dest;
    dest.x = pos.x - camera.x;
    dest.y = pos.y - camera.y;
    dest.w = pos.w;
    dest.h = pos.h;

    SDL_RenderCopyEx(renderer, tex, &src, &dest, angle, NULL, SDL_FLIP_NONE);
}
void renderSpriteAbsolute(SDL_Texture *tex, SDL_Rect pos, double angle, SDL_Renderer *renderer)
{
    SDL_Rect dest;
    dest.x = pos.x;
    dest.y = pos.y;
    dest.w = pos.w;
    dest.h = pos.h;

    SDL_RenderCopyEx(renderer, tex, NULL, &dest, angle, NULL, SDL_FLIP_NONE);
}

void initFonts()
{
    fontSmall = TTF_OpenFont("./assets/fonts/MegamaxJonathanToo-YqOq2.ttf", 18);
    fontBig = TTF_OpenFont("./assets/fonts/MegamaxJonathanToo-YqOq2.ttf", 60);
}

/// @brief
/// Renders given text on screen so that its geometric center
/// is in the point specified by `pos`
/// @param pos
/// @param renderer
/// @param font
/// @param text
/// @param color
void renderText(SDL_Point pos, SDL_Renderer *renderer, TTF_Font *font, char *text, SDL_Color color)
{

    SDL_Surface *textSurface = TTF_RenderText_Blended_Wrapped(font, text, color, 0);

    if (textSurface == NULL)
        return;

    SDL_Texture *renderedText = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textPos = (SDL_Rect){.x = pos.x - textSurface->w / 2,
                                  .y = pos.y - textSurface->h / 2,
                                  .w = textSurface->w,
                                  .h = textSurface->h};

    SDL_FreeSurface(textSurface);

    renderSpriteAbsolute(renderedText, textPos, 0, renderer);

    SDL_DestroyTexture(renderedText);
}

void renderRect(SDL_Rect rect, SDL_Rect camera, SDL_Renderer *renderer)
{

    SDL_Rect dest = rect;

    dest.x = rect.x - camera.x;
    dest.y = rect.y - camera.y;

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_RenderDrawRect(renderer, &dest);
}

int min(int a, int b)
{

    if (a > b)
        return b;
    return a;
}

int max(int a, int b)
{

    if (a > b)
        return a;
    return b;
}

int checkCollision(SDL_Rect a, SDL_Rect b)
{

    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    if (bottomA <= topB)
    {
        return 0;
    }

    if (topA >= bottomB)
    {
        return 0;
    }

    if (rightA <= leftB)
    {
        return 0;
    }

    if (leftA >= rightB)
    {
        return 0;
    }

    return 1;
}