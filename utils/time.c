#include "time.h"
#include <stdio.h>
#include <time.h>

void printCurrentTime() {
  struct timespec ts;
  struct tm t;
  if (clock_gettime(CLOCK_REALTIME, &ts) == 0) {
    time_t seconds = ts.tv_sec;
    localtime_r(&seconds, &t);
    int hours = t.tm_hour;
    int minutes = t.tm_min;
    long nanoseconds = ts.tv_nsec;
    printf("%02d:%02d:%02ld.%ld ", hours, minutes, seconds % 60, nanoseconds);
  } else {
    perror("Unable get current time ");
  }
}
