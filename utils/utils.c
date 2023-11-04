#ifdef _WIN32
#include <windows.h>
#elif __APPLE__

#include <sys/sysctl.h>
#include <string.h>
#include <stdio.h>

#else
#include <unistd.h>
#endif

// path example in Windows
//C:\Documents\Newsletters\Summer2018.pdf
// path example in Posix {macOS, Linux}
///Users/username/CLionProjects/HSE_zip/utils/utils.c

int getFilePath(int filenameLen, char* path, char* filename) {
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
