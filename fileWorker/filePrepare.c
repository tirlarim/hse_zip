#include <stdio.h>
#include <string.h>
#include "filePrepare.h"

// Step One

void fileRead(FileInfo* fileInfo);

void init(FileInfo* fileInfo) {
  fileInfo->size = 0;
  fileInfo->file = fopen("../testData/text_20byte.txt", "rb");
  fseek(fileInfo->file, 0, SEEK_END);
  fileInfo->size = ftell(fileInfo->file);
  fseek(fileInfo->file, 0, SEEK_SET);
  fclose(fileInfo->file);
  fileRead(fileInfo);
}

void fileRead(FileInfo* fileInfo) {
  fileInfo->file = fopen("../testData/text_20byte.txt", "r");
  unsigned long long readIndex = 0;
  int buffer = getc(fileInfo->file);
  while (buffer != EOF) {
    fileInfo->content[readIndex] = (char)buffer;
    buffer = fgetc(fileInfo->file);
    readIndex++;
  }
  fclose(fileInfo->file);
  fileInfo->content[readIndex] = '\0';
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
  printf("%s\n", fileInfo->content);
}
