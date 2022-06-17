#include <string.h>
#include <stdio.h>
#include "tree_list/tree_list.h"
#include "fileWorker/filePrepare.h"
#include "utils/printColors.h"

int main(int argc, char* argv[]) {
  if (argc < 3) {printf("wrong paths\n");exit(1);}
  NODE* binTree = NULL;
  char fileNameInput[100], fileNameOutput[100];
  strcpy(fileNameInput, argv[1]); strcpy(fileNameOutput, argv[2]);
  printf("working on: %s\n", fileNameInput);
  init_tree(binTree, fileNameInput, fileNameOutput);
  if (__APPLE__) {
    checkFileSize(fileNameInput, fileNameOutput);
    printf(ANSI_COLOR_YELLOW"check sha 256 sum:"ANSI_COLOR_RESET" ");
    checkFileHash(fileNameInput, fileNameOutput);
  }
  return 0;
}
