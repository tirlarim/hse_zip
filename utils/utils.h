#ifndef HSE_ZIP_UTILS_H
#define HSE_ZIP_UTILS_H

#include <time.h>

#define FILENAME_PATH_LEN 100
#define ENABLE_PROGRESS

unsigned int getNumberOfCores();
int getFilePath(int filenameLen, char* path, const char* filename);
int getFilename(int filenameLen, char* filename, const char* path);
void printLog(const char* format, ...);
void printProgress(unsigned long bitsAll, unsigned long bitsDecode, clock_t end, clock_t start);
unsigned long getFileSize(const char* filename);

#endif //HSE_ZIP_UTILS_H
