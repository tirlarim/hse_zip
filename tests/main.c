#include <stdio.h>
#include <stdlib.h>
#include "../fileWorker/filePrepare.h"
#include "../utils/printColors.h"
#include "../tree_list/encode.h"
#include "../tree_list/decode.h"

#define TEST_COUNT 3


char filenames[][FILENAME_MAX] = {
    "./testDataInput/1mb.png",
    "./testDataInput/2mb.jpg",
    "./testDataInput/11mb.png",
//    "./testDataInput/example.txt", // error
//    "./testDataInput/test.docx",
//    "./testDataInput/test.txt",
//    "./testDataInput/test_jpg.jpg",
//    "./testDataInput/video.webm",
};
char archivePath[] = "./testDataOutput/answer.hse_zip";

int main() {
  for (int i = 0; i < TEST_COUNT; ++i) {
    printf("working on: %s\n", filenames[i]);
    encodeArchive(filenames[i], archivePath);
    decodeArchive(archivePath);
#ifdef __APPLE__
    checkFileSize(filenames[i], archivePath);
    printf(ANSI_COLOR_YELLOW"check sha 256 sum:"ANSI_COLOR_RESET" ");
    checkFileHash(filenames[i], archivePath);
#endif
  }
}