#include "gtest/gtest.h"
#include "./main.h"

//extern "C" {
//#include "../tree_list/encode.h"
//#include "../tree_list/decode.h"
//}


TEST(Text, 1Byte) {
  std::string filename = "text_1byte.txt";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Text, 2Byte) {
  std::string filename = "text_2byte.txt";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Text, 3Byte) {
  std::string filename = "text_3byte.txt";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Text, 4Byte) {
  std::string filename = "text_4byte.txt";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Text, 5Byte) {
  std::string filename = "text_5byte.txt";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Text, 10Byte) {
  std::string filename = "text_10byte.txt";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Text, 20Byte) {
  std::string filename = "text_20byte.txt";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Text, 50Byte) {
  std::string filename = "text_50byte.txt";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Text, 100Byte) {
  std::string filename = "text_100byte.txt";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Text, 1KB) {
  std::string filename = "text_1000byte.txt";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Text, 34KB) {
  std::string filename = "example.txt";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Text, 100KB) {
  std::string filename = "text_100000byte.txt";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Text, AAAAA) {
  std::string filename = "text_A.txt";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}

TEST(Text, AB) {
  std::string filename = "1mb_ab.txt";
  std::string currentHash, correctHash;
  makeTest(filename, &correctHash, &currentHash);
  EXPECT_EQ(currentHash, correctHash);
}
