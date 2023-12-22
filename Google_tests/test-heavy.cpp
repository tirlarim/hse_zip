#include "gtest/gtest.h"
#include "./main.h"

#ifdef TEST_HEAVY
TEST(Heavy_Binary, 789mb_video) {
  std::string filename = "video.webm";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}
#endif
