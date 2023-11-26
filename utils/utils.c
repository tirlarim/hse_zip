#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <sys/sysctl.h>
#include <string.h>
#include <stdio.h>
#else
#include <unistd.h>
#endif
#include "./time.h"
#include "./printColors.h"
#include "../tree_list/tree_list.h"

int getFilePath(int filenameLen, char* path, const char* filename) {
  strcpy(path, filename);
#ifdef WIN32
  char divideSymbol = '\\';
#else
  char divideSymbol = '/';
#endif
  int startIndex;
  for (startIndex = 0; path[startIndex] != '\0' && startIndex < filenameLen; ++startIndex) {}
  for (; startIndex >= 0; --startIndex) {
    if (path[startIndex] == divideSymbol) break;
    path[startIndex] = '\0';
  }
  return startIndex+1;
}

int getFilename(int filenameLen, char* filename, const char* path) {
#ifdef WIN32
  char divideSymbol = '\\';
#else
  char divideSymbol = '/';
#endif
  int pathIndex, filenameIndex;
  for (pathIndex = 0; path[pathIndex] != '\0' && pathIndex < filenameLen; ++pathIndex) {}
  for (; path[pathIndex] != divideSymbol && pathIndex >= 0; --pathIndex) {}
  ++pathIndex;
  for (filenameIndex = 0; path[pathIndex] != '\0' && pathIndex < filenameLen; ++pathIndex, ++filenameIndex) {
    filename[filenameIndex] = path[pathIndex];
  }
  return filenameIndex;
}


#ifdef WIN32
unsigned int getNumberOfCores() {
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  return sysinfo.dwNumberOfProcessors;
}
#elif __APPLE__

unsigned int getNumberOfCores() {
  int nm[2];
  size_t len = 4;
  uint32_t count;
  nm[0] = CTL_HW;
  nm[1] = HW_AVAILCPU;
  sysctl(nm, 2, &count, &len, NULL, 0);
  if (count < 1) {
    nm[1] = HW_NCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);
    if (count < 1) count = 1;
  }
  return count;
}

#else
unsigned int getNumberOfCores() {
  return sysconf(_SC_NPROCESSORS_ONLN);
}
#endif

void printLog(const char* message) {
  if (PRINTF_DEBUG) {
    if (__APPLE__) printf(ANSI_COLOR_YELLOW);
    printCurrentTime();
    printf("%s", message);
    if (__APPLE__) printf(ANSI_COLOR_RESET);
  }
}

//int val = (int) (percentage * 100);
//int leftPad = (int) (percentage * PBWIDTH);
//int rightPad = PBWIDTH - leftPad;
//sec = percentage ? sec : 0;

//double progress = ((double)decodeBits / (double)decodeFileSizeBytes) + 0.01;
//printProgress(progress, (long long)(((double)(loopEnd - loopStart) / (CLOCKS_PER_SEC)) * (((double)1 - progress) * 100)));



// rewrite sec
void printProgress(unsigned long bitsAll, unsigned long bitsDecode, clock_t end, clock_t start) {
  double percentage = ((double)bitsDecode / (double)bitsAll) + 0.01;
  unsigned char percentageAsInteger = (unsigned char)(percentage * 100);
  unsigned long sec = (unsigned long)((double)(end - start) / CLOCKS_PER_SEC * (1.0 - percentage) * 100);
  unsigned short leftPad = (unsigned short)(percentage * PBWIDTH);
  int rightPad = PBWIDTH - leftPad;
  sec = percentage / 100 ? sec : 0;
  printf("\r%3d%% [%.*s%*s] est. time: ~ ", percentageAsInteger, leftPad, PBSTR, rightPad, "");
  if (sec != -1) {
    if (percentage == 100) sec = 0;
    if (sec >= 604800) {
      printf("%lu weeks ", sec/604800);
    } if (sec >= 86400) {
      printf("%lu days ", sec/86400%8);
    } if (sec >= 3600) {
      printf("%lu hours ", sec/3600%24);
    } if (sec >= 60) {
      printf("%lu minutes ", sec/60%60);
    } else {
      printf("%lu sec  ", sec%60);
    }
  } else {
    printf("? sec");
  }
  fflush(stdout);
}

