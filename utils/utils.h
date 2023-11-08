#ifndef HSE_ZIP_UTILS_H
#define HSE_ZIP_UTILS_H

#define FILENAME_PATH_LEN 100

unsigned int getNumberOfCores();
int getFilePath(int filenameLen, char* path, char* filename);
int getFilename(int filenameLen, char* filename, const char* path);
void printLog(const char* message);
void printProgress(double percentage, long long sec);

#endif //HSE_ZIP_UTILS_H
