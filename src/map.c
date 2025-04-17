#include <map.h>

SDL_Texture *normalTile;
SDL_Texture *borderTile;

unsigned char tiles[TOTAL_HEIGHT / 80][TOTAL_WIDTH / 80];

void initMap(SDL_Renderer *renderer)
{

    normalTile = textureManager[Tile];
    borderTile = textureManager[BorderTile];

    int pX, pY;

    for (int row = 0; row < TOTAL_HEIGHT / 80; row++)
    {
        for (int col = 0; col < TOTAL_WIDTH / 80; col++)
        {
            pX = col * 80;
            pY = row * 80;
            if (pX < SCREEN_WIDTH / 2 || pX >= SCREEN_WIDTH / 2 + LEVEL_WIDTH)
                tiles[row][col] = 1;

            else if (pY < SCREEN_HEIGHT / 2 || pY >= SCREEN_HEIGHT / 2 + LEVEL_HEIGHT)
                tiles[row][col] = 1;

            else
                tiles[row][col] = 0;
        }
    }
}

void drawMap(SDL_Renderer *renderer, SDL_Rect *camera)
{

    SDL_Rect dest;
    int pX, pY;

    for (int row = 0; row < TOTAL_HEIGHT / 80; row++)
    {
        for (int col = 0; col < TOTAL_WIDTH / 80; col++)
        {
            pX = col * 80;
            pY = row * 80;

            if (pX + 80 >= camera->x && pX <= camera->x + camera->w)
            {
                if (pY + 80 >= camera->y && pY <= camera->y + camera->h)
                {
                    dest.x = pX - camera->x;
                    dest.y = pY - camera->y;
                    dest.w = 80;
                    dest.h = 80;

                    switch (tiles[row][col])
                    {
                    case 0:
                        SDL_RenderCopy(renderer, normalTile, NULL, &dest);
                        break;
                    case 1:
                        SDL_RenderCopy(renderer, borderTile, NULL, &dest);
                        break;
                    }
                }
            }
        }
    }
}