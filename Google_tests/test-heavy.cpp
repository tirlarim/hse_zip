#include "gtest/gtest.h"
#include "./utils/test-utils.hpp"
#include "./main.h"

extern "C" {
#include "../tree_list/encode.h"
#include "../tree_list/decode.h"
}

TEST(Heavy_Binary, 789mb_video) {
  std::string filename = "video.webm";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}
