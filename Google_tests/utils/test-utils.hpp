#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <openssl/evp.h>

unsigned long simple_hash(const std::string& path);
void calculate_sha256(const std::string &path, std::string* ans);

#endif //TEST_UTILS_H
