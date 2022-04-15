#include <string.h>
#include "tree_list/tree_list.h"
#include "fileWorker/filePrepare.h"

int main() {
  NODE* binTree = NULL;
  char fileNameInput[] = "../testDataInput/text_1000byte.txt",
       fileNameOutput[] = "../testDataOutput/answer.hse_zip";
  printf("working on: %s\n", fileNameInput);
  init_tree(binTree, fileNameInput, fileNameOutput);
  checkFileSize(fileNameInput, fileNameOutput);
  return 0;
}
