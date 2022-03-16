#include <string.h>
#include "tree_list/tree_list.h"

int main() {
  NODE* binTree = NULL;
  char fileNameInput[] = "../testData/example.txt",
       fileNameOutput[] = "../testDataOutput/answer.hse_zip",
       fileNameOutputUnzip[] = "../testDataOutput/answer.txt";
  initTree(binTree, fileNameInput, fileNameOutput, fileNameOutputUnzip);
  return 0;
}
