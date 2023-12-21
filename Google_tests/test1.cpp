#include <filesystem>
#include "gtest/gtest.h"
#include "./utils/test-utils.hpp"
#include "./consts.h"

extern "C" {
#include "../tree_list/encode.h"
#include "../tree_list/decode.h"
}

TEST(Binary, 13kb_docx) {
  std::string currentHash, correctHash;
  std::string filename = "test.docx";
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

TEST(Binary, 77kb_pic) {
  std::string currentHash, correctHash;
  std::string filename = "test_jpg.jpg";
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

TEST(Binary, 1mb_pic) {
  std::string currentHash, correctHash;
  std::string filename = "1mb.png";
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

TEST(Binary, 2mb_pic) {
  std::string currentHash, correctHash;
  std::string filename = "2mb.jpg";
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

TEST(Binary, 11mb_pic) {
  std::string currentHash, correctHash;
  std::string filename = "11mb.png";
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
