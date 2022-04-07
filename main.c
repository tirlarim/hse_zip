#include <string.h>
#include "tree_list/tree_list.h"
#include "fileWorker/filePrepare.h"

int main() {
  NODE* binTree = NULL;
  char fileNameInput[] = "../testDataInput/1mb.png",
       fileNameOutput[] = "../testDataOutput/answer.hse_zip";
  //   buffer file path at tree_list.c line 4
  init_tree(binTree, fileNameInput, fileNameOutput);
  checkFileSize(fileNameInput, fileNameOutput);
  return 0;
}
//to remove trash bits print comment tree_list.c line 223 and uncomment line 224
