#ifndef LOGTOFILE_H
#define LOGTOFILE_H
#include <time.h>

void logGameStartToFile();

void logGameEndToFile();

extern time_t timeAtStart;

#endif