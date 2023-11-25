#ifndef HSE_ZIP_UTILS_H
#define HSE_ZIP_UTILS_H

#include <time.h>

#define FILENAME_PATH_LEN 100

unsigned int getNumberOfCores();
int getFilePath(int filenameLen, char* path, const char* filename);
int getFilename(int filenameLen, char* filename, const char* path);
void printLog(const char* message);
void printProgress(unsigned long bitsAll, unsigned long bitsDecode, clock_t end, clock_t start);

#endif //HSE_ZIP_UTILS_H
