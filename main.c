#include <stdio.h>
#include <stdlib.h>
#include "fileWorker/filePrepare.h"
#include "utils/printColors.h"
#include "tree_list/encode.h"
#include "tree_list/decode.h"

int main(int argc, char* argv[]) {
  if (argc < 3) {printf("wrong paths\n");exit(1);}
  printf("working on: %s\n", argv[1]);
  encodeArchive(argv[1], argv[2]);
  decodeArchive(argv[2]);
#ifdef __APPLE__
  checkFileSize(argv[1], argv[2]);
  printf(ANSI_COLOR_YELLOW"check sha 256 sum:"ANSI_COLOR_RESET" ");
  checkFileHash(argv[1], argv[2]);
#endif
  return 0;
}

// TODO: add async encode/decode
// TODO: add semaphores to check limits in zip app
// FIXME: encode doesn't work with txt file
//        i think bug happens if we cant find all 0-255 bytes in file (check it)
//
