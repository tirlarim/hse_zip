#include <filesystem>
#include "gtest/gtest.h"
#include "./utils/test-utils.hpp"
#include "./consts.h"

extern "C" {
#include "../tree_list/encode.h"
#include "../tree_list/decode.h"
}


TEST(Text, 1Byte) {
  std::string currentHash, correctHash;
  std::string filename = "text_1byte.txt";
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

TEST(Text, 2Byte) {
  std::string currentHash, correctHash;
  std::string filename = "text_2byte.txt";
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

TEST(Text, 3Byte) {
  std::string currentHash, correctHash;
  std::string filename = "text_3byte.txt";
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

TEST(Text, 4Byte) {
  std::string currentHash, correctHash;
  std::string filename = "text_4byte.txt";
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

TEST(Text, 5Byte) {
  std::string currentHash, correctHash;
  std::string filename = "text_5byte.txt";
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

TEST(Text, 10Byte) {
  std::string currentHash, correctHash;
  std::string filename = "text_10byte.txt";
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

TEST(Text, 20Byte) {
  std::string currentHash, correctHash;
  std::string filename = "text_20byte.txt";
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

TEST(Text, 50Byte) {
  std::string currentHash, correctHash;
  std::string filename = "text_50byte.txt";
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

TEST(Text, 100Byte) {
  std::string currentHash, correctHash;
  std::string filename = "text_100byte.txt";
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

TEST(Text, 1KB) {
  std::string currentHash, correctHash;
  std::string filename = "text_1000byte.txt";
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

TEST(Text, 34KB) {
  std::string currentHash, correctHash;
  std::string filename = "example.txt";
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

TEST(Text, 100KB) {
  std::string currentHash, correctHash;
  std::string filename = "text_100000byte.txt";
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

TEST(Text, AAAAA) {
  std::string currentHash, correctHash;
  std::string filename = "text_A.txt";
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

TEST(Text, AB) {
  std::string currentHash, correctHash;
  std::string filename = "1mb_ab.txt";
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
