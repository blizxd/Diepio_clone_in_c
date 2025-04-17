#include <logtofile.h>
#include <stdio.h>

time_t timeAtStart;

void logGameStartToFile()
{

    FILE *file = fopen("log.txt", "a");
    if (file == NULL)
        return;

    time(&timeAtStart);

    char date[50];
    strftime(date, 50, "%B %d %H:%M", localtime(&timeAtStart));

    fprintf(file, "\nGame started at %s\n", date);

    fclose(file);
}

void logGameEndToFile()
{
    FILE *file = fopen("log.txt", "a");
    if (file == NULL)
        return;
    time_t finishTime = time(NULL);

    int totalSeconds = (int)difftime(finishTime, timeAtStart);

    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    fprintf(file, "The game worked for %d minutes and %d seconds \n", minutes, seconds);

    fclose(file);
}