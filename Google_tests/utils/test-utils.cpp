#include <fstream>
#include <iostream>
#include <vector>
#include <openssl/evp.h>

unsigned long simple_hash(const std::string &path) {
  unsigned long hash = 0;
  char data;
  std::ifstream file(path, std::ios::binary);
  if (!file) {
    std::cerr << "Unable to open file: " << path << std::endl;
    return 0;
  }
  while (file.get(data)) {
    hash += static_cast<unsigned char>(data);
  }
  file.close();
  return hash;
}
// God, why is it so hard
void calculate_sha256(const std::string &path, std::string* ans) {
  unsigned char hash[EVP_MAX_MD_SIZE];
  unsigned int lengthOfHash = 0;
  std::ifstream file(path, std::ifstream::binary);
  std::vector<unsigned char> buffer(1024);
  if (!file) {
    std::cerr << "Unable to open file: " << path << std::endl;
    return;
  }
  EVP_MD_CTX* context = EVP_MD_CTX_new();
  if (!context) {
    std::cerr << "Hash context creation error" << std::endl;
    return;
  }
  if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1) {
    EVP_MD_CTX_free(context);
    std::cerr << "Hash context initialization error" << std::endl;
    return;
  }
  while (file.read(reinterpret_cast<char*>(buffer.data()), static_cast<long>(buffer.size()))) {
    if (EVP_DigestUpdate(context, buffer.data(), file.gcount()) != 1) {
      EVP_MD_CTX_free(context);
      std::cerr << "Hash update error" << std::endl;
      return;
    }
  }
  if (EVP_DigestFinal_ex(context, hash, &lengthOfHash) != 1) {
    EVP_MD_CTX_free(context);
    std::cerr << "Hashing error" << std::endl;
    return;
  }
  EVP_MD_CTX_free(context);

  std::cout << path << std::endl;
  for (unsigned int i = 0; i < lengthOfHash; i++) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << (0xff & hash[i]);
  }
  std::cout << std::endl;

  ans->reserve(lengthOfHash*2);
  for (int i = 0; i < lengthOfHash; ++i) {
    char tmpBf[3]; // ff + 0x00
    snprintf(tmpBf, 3, "%.2x", hash[i]);
    ans->append(tmpBf);
  }
}
