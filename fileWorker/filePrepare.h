#ifndef HSE_ZIP_FILE_PREPARE_H
#define HSE_ZIP_FILE_PREPARE_H

#define FILE_SIZE 100 // program don't work if size <= 1000
#define FILE_NAME_SIZE 100
#define FILE_COUNT_ARR_LEN 256

typedef struct {
  unsigned long long size;
  unsigned long long symbolsCountArr[FILE_COUNT_ARR_LEN][2]; //first -> symbol as hex (int) | second -> symbol count
  unsigned char content[FILE_SIZE];
  char fileName[FILE_NAME_SIZE];
  FILE* file;
}FileInfo;

void init(FileInfo* fileInfo);
void printFileAsHex(FileInfo* fileInfo);
void printFileSize(FileInfo* fileInfo);
void printFileAsText(FileInfo* fileInfo);
void printSymbolsCountArr(FileInfo* fileInfo);
void checkFileSize(const char* filenameInput, const char* filenameOutput);
void checkFileHash(const char* filenameInput, const char* filenameOutput);
unsigned long simple_hash(const char* path);

#endif //HSE_ZIP_FILE_PREPARE_H
