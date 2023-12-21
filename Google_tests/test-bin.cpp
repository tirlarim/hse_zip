#include "gtest/gtest.h"
#include "./main.h"

TEST(Binary, 13kb_docx) {
  std::string filename = "test.docx";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);  
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Binary, 77kb_pic) {
  std::string filename = "test_jpg.jpg";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Binary, 1mb_pic) {
  std::string filename = "1mb.png";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Binary, 2mb_pic) {
  std::string filename = "2mb.jpg";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Binary, 11mb_pic) {
  std::string filename = "11mb.png";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}
