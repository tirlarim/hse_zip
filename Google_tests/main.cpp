#include "./main.h"
#include "./utils/test-utils.hpp"

extern "C" {
#include "../tree_list/encode.h"
#include "../tree_list/decode.h"
}

void makeTest(const std::string& filename, std::string* correctHash, std::string* currentHash) {
  std::string pathIn = dirInput + filename;
  std::string pathOut = dirOutput + filename;
  char pathInArr[MAX_PATH_LEN] = {0};
  char pathZipArr[MAX_PATH_LEN] = {0};
  strcpy(pathInArr, pathIn.data());
  strcpy(pathZipArr, pathZip.data());
  encodeArchive(pathInArr, pathZipArr);
  decodeArchive(pathZipArr);
  calculate_sha256(dirInput + filename, correctHash);
  calculate_sha256(dirOutput + filename, currentHash);
}
