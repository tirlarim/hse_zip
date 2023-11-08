#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree_list/tree_list.h"
#include "fileWorker/filePrepare.h"
#include "utils/printColors.h"
#include "utils/utils.h"
#include "tree_list/encode.h"
#include "tree_list/decode.h"

int main(int argc, char* argv[]) {
  if (argc < 3) {printf("wrong paths\n");exit(1);}
  char fileNameInput[FILENAME_PATH_LEN], fileNameOutput[FILENAME_PATH_LEN];
  memset(fileNameInput, '\0', sizeof fileNameInput);
  memset(fileNameOutput, '\0', sizeof fileNameOutput);
  strcpy(fileNameInput, argv[1]); strcpy(fileNameOutput, argv[2]);
  printf("working on: %s\n", fileNameInput);
  encodeArchive(fileNameInput, fileNameOutput);
  decodeArchive(fileNameOutput);
#ifdef __APPLE__
  checkFileSize(fileNameInput, fileNameOutput);
  printf(ANSI_COLOR_YELLOW"check sha 256 sum:"ANSI_COLOR_RESET" ");
  checkFileHash(fileNameInput, fileNameOutput);
#endif
  return 0;
}

// TODO: add semaphores to check limits in zip app