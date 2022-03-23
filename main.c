#include <string.h>
#include "tree_list/tree_list.h"
#include "fileWorker/filePrepare.h"

int main() {
  NODE* binTree = NULL;
  char fileNameInput[] = "../testDataInput/example.txt",
       fileNameOutput[] = "../testDataOutput/answer.hse_zip";
  initTree(binTree, fileNameInput, fileNameOutput);
  checkFileSize(fileNameInput, fileNameOutput);
  return 0;
}
