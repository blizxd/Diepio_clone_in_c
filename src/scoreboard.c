#include <helper.h>
#include <math.h>
#include <menuBackground.h>
#include <player.h>
#include <scoreboard.h>
#include <string.h>

typedef struct
{
    int score;
    time_t rawTime;
} PlayerScoreData;

PlayerScoreData *scoreList = NULL;
int numberOfRecords = 0;
int scoreListStartIndex = 0;

void writeScoreToFile()
{

    FILE *file = fopen(SCOREBOARD_FILE, "a+b");

    fread(&numberOfRecords, sizeof(int), 1, file);

    numberOfRecords++;

    freopen(SCOREBOARD_FILE, "r+b", file);

    fwrite(&numberOfRecords, sizeof(int), 1, file);

    freopen(SCOREBOARD_FILE, "ab", file);
    time_t rawTime = time(NULL);
    fwrite(&player.currentScore, sizeof(int), 1, file);
    fwrite(&rawTime, sizeof(time_t), 1, file);

    fclose(file);
}

void readScoreFromFile()
{

    FILE *file = fopen(SCOREBOARD_FILE, "rb");

    if (file != NULL)
    {

        fread(&numberOfRecords, sizeof(int), 1, file);

        if (scoreList != NULL)
            free(scoreList);

        scoreList = malloc(sizeof(PlayerScoreData) * numberOfRecords);

        for (int i = 0; i < numberOfRecords; i++)
        {
            fread(&scoreList[i].score, sizeof(int), 1, file);
            fread(&scoreList[i].rawTime, sizeof(time_t), 1, file);
        }

        sortScoreList();
        fclose(file);
    }
}

void switchToScoreboard()
{
    currentScene = SCOREBOARD;
    scoreListStartIndex = 0;
    readScoreFromFile();
}

void sortScoreList()
{

    for (int i = 0; i < numberOfRecords; i++)
    {
        for (int j = 0; j < numberOfRecords - i - 1; j++)
        {
            if (scoreList[j].score <= scoreList[j + 1].score)
            {
                PlayerScoreData temp = scoreList[j];
                scoreList[j] = scoreList[j + 1];
                scoreList[j + 1] = temp;
            }
        }
    }
}

void displayScoreboard()
{

    renderMenuBackground();

    SDL_Color color = {0, 0, 0};

    SDL_Point pos = (SDL_Point){.x = SCREEN_WIDTH / 2, .y = SCREEN_HEIGHT / 2 - 300};

    char text[50] = "Player scoreboard";

    renderText(pos, renderer, fontBig, text, color);

    int maxIndex = min(scoreListStartIndex + 5, numberOfRecords);

    for (int i = scoreListStartIndex; i < maxIndex; i++)
    {
        char record[50];
        snprintf(record, 50, "%d - ", scoreList[i].score);

        char date[50];
        strftime(date, 50, "%B %d %H:%M", localtime(&scoreList[i].rawTime));

        strncat(record, date, 50 - strlen(record) - 1);

        pos.y += 100;

        renderText(pos, renderer, fontBig, record, color);
    }

    snprintf(text, 50, "Displaying %d of %d records", min(scoreListStartIndex + 5, numberOfRecords),
             numberOfRecords);

    pos.y += 100;

    renderText(pos, renderer, fontBig, text, color);

    SDL_RenderPresent(renderer);
}

void handleScoreBoardInput(SDL_Event e)
{
    if (e.type == SDL_KEYDOWN)
    {
        if (e.key.keysym.sym == SDLK_ESCAPE)
            currentScene = MAIN_MENU;
        if (e.key.keysym.sym == SDLK_DOWN && scoreListStartIndex < numberOfRecords - 5)
            scoreListStartIndex++;
        if (e.key.keysym.sym == SDLK_UP && scoreListStartIndex > 0)
        {
            scoreListStartIndex--;
        }
    }
}