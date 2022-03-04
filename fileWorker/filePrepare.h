#ifndef HSE_ZIP_FILE_PREPARE_H
#define HSE_ZIP_FILE_PREPARE_H

#define FILE_SIZE 1000000
#define FILE_NAME_SIZE 100
#define FILE_COUNT_ARR_LEN 256
//#define CODE_SIZE 1

typedef struct {
  unsigned long long size;
  unsigned long long symbolsCountArr[FILE_COUNT_ARR_LEN];
  int content[FILE_SIZE];
  char fileName[FILE_NAME_SIZE];
  FILE* file;
}FileInfo;

typedef struct BIN_TREE {
  unsigned char symbol;
  unsigned char isSymbol;
  unsigned int symbolCount;
  unsigned char symbolCode;
  int deepLvl;
  struct BIN_TREE *left, *right, *top;
}BinTree;


void init(FileInfo* fileInfo);
void printFileAsHex(FileInfo* fileInfo);
void printFileSize(FileInfo* fileInfo);
void printFileAsText(FileInfo* fileInfo);
void printSymbolsCountArr(FileInfo* fileInfo);

#endif //HSE_ZIP_FILE_PREPARE_H
