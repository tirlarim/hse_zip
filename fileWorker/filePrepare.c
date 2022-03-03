#include <stdio.h>
#include "filePrepare.h"

// Step One

void init(FileInfo* fileInfo) {
  fileInfo->file = fopen("../testData/text_20byte.txt", "rb");
  fseek(fileInfo->file, 0, SEEK_END);
  fileInfo->size = ftell(fileInfo->file);
  fseek(fileInfo->file, 0, SEEK_SET);
  fclose(fileInfo->file);
}

void printFileSize(FileInfo* fileInfo) {
  printf("Size of file: %llu bytes.\n", fileInfo->size);
}

void printFileAsHex(FileInfo* fileInfo) {
  fileInfo->file = fopen("../testData/text_20byte.txt", "rb");
  int a;
  for (int i = 0; i < fileInfo->size; i++) {
    fread(&a, 1, 1, fileInfo->file);
    if (i % 5 == 0 && i != 0) {
      printf("\n");
    }
    printf("0x%x -> %c\t", a, a);
  }
  printf("\n");
  fclose(fileInfo->file);
}