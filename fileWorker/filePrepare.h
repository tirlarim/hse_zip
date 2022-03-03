#ifndef HSE_ZIP_FILE_PREPARE_H
#define HSE_ZIP_FILE_PREPARE_H

#define FILE_SIZE 1000000
#define FILE_NAME_SIZE 100

typedef struct {
  unsigned long long size;
  unsigned long long symbolsCountArr[256];
  char content[FILE_SIZE], fileName[FILE_NAME_SIZE];
  FILE* file;
}FileInfo;

void init(FileInfo* fileInfo);
void printFileAsHex(FileInfo* fileInfo);
void printFileSize(FileInfo* fileInfo);
void printFileAsText(FileInfo* fileInfo);

#endif //HSE_ZIP_FILE_PREPARE_H
