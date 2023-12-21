#include <filesystem>
#include "gtest/gtest.h"
#include "./utils/test-utils.hpp"
#include "./consts.h"

extern "C" {
#include "../tree_list/encode.h"
#include "../tree_list/decode.h"
}

TEST(Heavy_Binary, 789mb_video) {
  std::string currentHash, correctHash;
  std::string filename = "video.webm";
  std::string pathIn = dirInput + filename;
  std::string pathOut = dirOutput + filename;
  char t1[100] = {0}, t2[100] = {0}, t3[100] = {0};
  strcpy(t1, pathIn.data());
  strcpy(t2, pathOut.data());
  strcpy(t3, pathZip.data());
  encodeArchive(t1, t3);
  decodeArchive(t3);
  calculate_sha256(dirInput + filename, &correctHash);
  calculate_sha256(dirOutput + filename, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}
