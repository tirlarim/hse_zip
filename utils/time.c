#include "time.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void printCurrentTime() {
  time_t rawTime;
  struct tm* timeInfo;
  char currentTime[9];
  time(&rawTime);
  timeInfo = localtime(&rawTime);
  strncpy(currentTime, &asctime(timeInfo)[11], 8);
  currentTime[8] = '\0';
  printf("%s ", currentTime);
}
