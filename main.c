#include <stdio.h>
#include "fileWorker/filePrepare.h"

int main() {
  FileInfo fileInfo;
  init(&fileInfo);
  printFileAsHex(&fileInfo);
  return 0;
}
