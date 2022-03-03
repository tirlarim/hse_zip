#include <stdio.h>
#include <string.h>
#include "fileWorker/filePrepare.h"

int main() {
  FileInfo fileInfo;
  char fileName[] = "../testData/test_jpg.jpg";
  memcpy(fileInfo.fileName, fileName, sizeof(fileName));
  init(&fileInfo);
//  printFileAsHex(&fileInfo);
//  printFileAsText(&fileInfo);
  printFileSize(&fileInfo);
  printSymbolsCountArr(&fileInfo);
  return 0;
}
