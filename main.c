#include <string.h>
#include "tree_list/tree_list.h"

int main() {
  NODE* binTree = NULL;
  char fileNameInput[] = "../testDataInput/example.txt",
       fileNameOutput[] = "../testDataOutput/answer.hse_zip";
  initTree(binTree, fileNameInput, fileNameOutput);
  return 0;
}
