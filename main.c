#include <string.h>
#include <stdio.h>
#include "tree_list/tree_list.h"
#include "fileWorker/filePrepare.h"
#include "utils/printColors.h"

int main(int argc, char* argv[]) {
  NODE* binTree = NULL;
  char fileNameInput[] = "../testDataInput/2mb.jpg",
       fileNameOutput[] = "../testDataOutput/answer.hse_zip";
  printf("working on: %s\n", fileNameInput);
  init_tree(binTree, fileNameInput, fileNameOutput);
  checkFileSize(fileNameInput, fileNameOutput);
  printf(ANSI_COLOR_YELLOW"check sha 256 sum:"ANSI_COLOR_RESET" ");
  checkFileHash(fileNameInput, fileNameOutput);
  return 0;
}

//before
//1mb ~ 2.8sec -> 2.56
//2mb ~ 4.83sec -> prof. 5sec => prepare=35% ~ getc=6% | find ans => 28% {decode = 92%}

//after
//1mb -> wrong res + 5.18sec