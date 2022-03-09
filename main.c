#include <stdio.h>
#include <string.h>
#include "fileWorker/filePrepare.h"
#include "tree_list/tree_list.h"

int main() {
  FileInfo fileInfo;
  char fileName[] = "../testData/text_20byte.txt";
  memcpy(fileInfo.fileName, fileName, sizeof(fileName));
  init(&fileInfo);
  printFileAsHex(&fileInfo);
  printFileAsText(&fileInfo);
  printFileSize(&fileInfo);
  printSymbolsCountArr(&fileInfo);
  initTree();
  return 0;
}
