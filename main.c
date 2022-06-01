#include <string.h>
#include <stdio.h>
#include "tree_list/tree_list.h"
#include "fileWorker/filePrepare.h"
#include "utils/printColors.h"

int main(int argc, char* argv[]) {
  NODE* binTree = NULL;
  char fileNameInput[] = "../testDataInput/256mbFile.wh",
       fileNameOutput[] = "../testDataOutput/answer.hse_zip";
  printf("working on: %s\n", fileNameInput);
  init_tree(binTree, fileNameInput, fileNameOutput);
  checkFileSize(fileNameInput, fileNameOutput);
  printf(ANSI_COLOR_YELLOW"check sha 256 sum:"ANSI_COLOR_RESET" ");
  checkFileHash(fileNameInput, fileNameOutput);
  return 0;
}
