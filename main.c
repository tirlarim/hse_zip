#include <stdio.h>
#include "fileWorker/filePrepare.h"

int main() {
  FileInfo fileInfo;
  init(&fileInfo);
//  printFileAsHex(&fileInfo);
  printFileAsText(&fileInfo);
  printFileSize(&fileInfo);
  return 0;
}
