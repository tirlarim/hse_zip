#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "filePrepare.h"

// Step One

void getFileSize(FileInfo* fileInfo);
void getFileContent(FileInfo* fileInfo);
void fillSymbolsCountArr(FileInfo* fileInfo);

void init(FileInfo* fileInfo) {
  getFileSize(fileInfo);
  getFileContent(fileInfo);
  fillSymbolsCountArr(fileInfo);
}

void getFileSize(FileInfo* fileInfo) {
  fileInfo->size = 0;
  fileInfo->file = fopen("../testData/text_20byte.txt", "rb");
  if (fileInfo->file == 0) {
    perror("Open error");
    exit(1);
  }
  fseek(fileInfo->file, 0, SEEK_END);
  fileInfo->size = ftell(fileInfo->file);
  fseek(fileInfo->file, 0, SEEK_SET);
  fclose(fileInfo->file);
}

void getFileContent(FileInfo* fileInfo) {
  fileInfo->file = fopen("../testData/text_20byte.txt", "rb");
  unsigned long long readIndex = 0;
  int buffer = getc(fileInfo->file);
  while (buffer != EOF) {
    fileInfo->content[readIndex] = buffer; // pls no write (char)buffer this is break all. we should use int
    printf("%d\t", fileInfo->content[readIndex]);
    buffer = fgetc(fileInfo->file);
    readIndex++;
  }
  fclose(fileInfo->file);
  fileInfo->content[readIndex] = 0;
}

void printFileSize(FileInfo* fileInfo) {
  printf("Size of file: %llu bytes.\n", fileInfo->size);
}

void printFileAsHex(FileInfo* fileInfo) {
  for (unsigned long long i = 0; i < fileInfo->size; i++) {
    if (i % 5 == 0 && i != 0) {
      printf("\n");
    }
    printf("0x%x -> %c\t", fileInfo->content[i], fileInfo->content[i]);
  }
  printf("\n");
}

void printFileAsText(FileInfo* fileInfo) {
  for (int i = 0; fileInfo->content[i] != 0 ; ++i) {
    printf("%c", (char)fileInfo->content[i]);
  }
  printf("\n");
}

void fillSymbolsCountArr(FileInfo* fileInfo) {
  for (unsigned long long i = 0; i < fileInfo->size; ++i) {
    fileInfo->symbolsCountArr[fileInfo->content[i]]++;
  }
}

void printSymbolsCountArr(FileInfo* fileInfo) {
  for (int i = 0; i < FILE_COUNT_ARR_LEN; ++i) {
    if (i%8==0) {
      printf("\n");
    }
    printf("0x%x -> %llu\t", i, fileInfo->symbolsCountArr[i]);
  }
}