#ifndef HSE_ZIP_MAIN_H
#define HSE_ZIP_MAIN_H

#include <string>

#define MAX_PATH_LEN 100

const std::string dirInput = "./testDataInput/";
const std::string dirOutput = "./testDataOutput/";
const std::string pathZip = "./testDataOutput/answer.hse_zip";

void makeTest(const std::string& filename, std::string* h1, std::string* h2);

#endif //HSE_ZIP_MAIN_H
