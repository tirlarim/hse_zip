#ifndef HSE_ZIP_FILEPREPARE_H
#define HSE_ZIP_FILEPREPARE_H

typedef struct {
  unsigned long long size;
  FILE* file;
}FileInfo;

void init(FileInfo* fileInfo);
void printFileAsHex(FileInfo* fileInfo);
void printFileSize(FileInfo* fileInfo);

#endif //HSE_ZIP_FILEPREPARE_H
