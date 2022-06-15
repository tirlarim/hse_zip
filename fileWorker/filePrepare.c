#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "filePrepare.h"
#include "../utils/printColors.h"
#include <stdbool.h>

void checkFileSize(char* filenameInput, char* filenameOutput) {
  long fileSizes[2] = {0};
  FILE* fileInput = fopen(filenameInput, "rb");
  FILE* fileOutput = fopen(filenameOutput, "rb");
  if (fileInput == 0 || fileOutput == 0) {
    perror("Open error");
    exit(3);
  }
  fseek(fileInput, 0, SEEK_END);
  fileSizes[0] = ftell(fileInput);
  fseek(fileInput, 0, SEEK_SET);
  fclose(fileInput);
  fseek(fileOutput, 0, SEEK_END);
  fileSizes[1] = ftell(fileOutput);
  fseek(fileOutput, 0, SEEK_SET);
  fclose(fileOutput);
  printf("file input: %ld bytes,  file output: %ld bytes\n", fileSizes[0], fileSizes[1]);
  fileSizes[0] > fileSizes[1] ?
  printf(ANSI_COLOR_GREEN"save %ld bytes -> %.2f%%"ANSI_COLOR_RESET"\n", fileSizes[0] - fileSizes[1], 100-((float)fileSizes[1]/(float)fileSizes[0])*100) :
  printf(ANSI_COLOR_RED"get %ld bytes -> %.2f%%"ANSI_COLOR_RESET"\n", fileSizes[1] - fileSizes[0], ((float)fileSizes[1]/(float)fileSizes[0])*100);
}

void checkFileHash(const char* filenameInput, char* filenameOutput) {
  char cmd1[200] = "shasum -a 256 ", cmd2[100] = "shasum -a 256 ", outputFileName[100] = {0};
  bool filesSame = true;
  strncat(outputFileName, filenameOutput, 18);
  strncat(outputFileName, &filenameInput[17], 100);
  strncat(cmd1, filenameInput, 100);
  strncat(cmd2, outputFileName, 100);

  FILE* fileOriginal = popen(cmd1, "r");
  FILE* fileUnzip = popen(cmd2, "r");
  char buffer1[65], buffer2[65];
  if (fscanf(fileOriginal, "%64s", buffer1)==1 && fscanf(fileUnzip, "%64s", buffer2)==1) {
//    buffer2[3] = 'f';
    printf(ANSI_COLOR_GREEN);
    for (int i = 0; i < 65; ++i) {
      if (buffer1[i] != buffer2[i]) {
        printf(ANSI_COLOR_RED);
        filesSame = false;
        break;
      }
    }
    filesSame ? printf("Good\n") : printf("Archive / Decode error\n");
    printf("%.64s -> %s\n", buffer1, filenameInput);
    printf("%.64s -> %s\n", buffer2, outputFileName);
  } else {
    printf("Error: can't get checksum :(\n");
  }
  pclose(fileOriginal);
  pclose(fileUnzip);
}
