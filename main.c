#include <string.h>
#include "tree_list/tree_list.h"
#include "fileWorker/filePrepare.h"

int main(int argc, char* argv[]) {
  NODE* binTree = NULL;
  char fileNameInput[] = "../testDataInput/1mb.png",
       fileNameOutput[] = "../testDataOutput/answer.hse_zip";
  printf("working on: %s\n", fileNameInput);
  init_tree(binTree, fileNameInput, fileNameOutput);
  checkFileSize(fileNameInput, fileNameOutput);
  return 0;
}
