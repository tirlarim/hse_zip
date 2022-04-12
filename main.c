#include <string.h>
#include "tree_list/tree_list.h"
#include "fileWorker/filePrepare.h"

int main() {
  NODE* binTree = NULL;
  char fileNameInput[] = "../testDataInput/11mb.png",
       fileNameOutput[] = "../testDataOutput/answer.hse_zip";
  init_tree(binTree, fileNameInput, fileNameOutput);
  checkFileSize(fileNameInput, fileNameOutput);
  return 0;
}
