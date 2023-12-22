#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "tree_list.h"
#include "decode.h"
#include "union.h"
#include "../utils/utils.h"

void decode(const char* archiveFilename);
void prepareByteBuffer(unsigned char* buffCode, FILE* fp, int* iRead, int arrSize, unsigned long* fileLen);
char findAnswer(CODES_AS_TREE* root, const unsigned char* arrayLen, int* readIndex);
void Add2Tree(CODES_AS_TREE* root, int arrayLen, int deepIndex, char* codes, char value);
void freeBinTree(CODES_AS_TREE* root);

typedef struct FileInfo {
  unsigned long size;
  unsigned long sizeUncompressed;
  unsigned int filenameLen;
  char filename[FILENAME_MAX];
}FILE_INFO;

void decodeArchive(char* fileNameOutput) {
  decode(fileNameOutput);
  printf("decode - DONE.\n");
}

CODES_AS_TREE* createBinTree(char codes[][CODE_SIZE], unsigned char headerSorted[BYTES_COUNT], int arrLen[CODE_SIZE]) {
  CODES_AS_TREE* root = (CODES_AS_TREE*)calloc(1, sizeof(*root));
  root->right = NULL;
  root->left = NULL;
  for (int i = 0; i < CODE_SIZE; ++i) {
    int arrCurrentLen = 0;
    if (*codes[headerSorted[i]] != -1) {
      Add2Tree(root, arrLen[i], arrCurrentLen, codes[(int)headerSorted[i]], (char)headerSorted[i]);
    }
  }
  return root;
}

void createCodeTable(char codes[][CODE_SIZE], char header[BYTES_COUNT * CODE_SIZE],
                     unsigned char headerSorted[BYTES_COUNT], int codesLens[CODE_SIZE], unsigned long* length) {
  int headerSortedIndex = 0;
  for (int i = 1; header[i] != '\0'; ++i, --*length) {
    unsigned char byte;
    if (header[i - 1] == ':' && (header[i] == 48 || header[i] == 49)) {
      byte = header[i - 2];
      headerSorted[headerSortedIndex] = byte;
      for (int j = 0; (header[i] == 48 || header[i] == 49) &&
                      (header[i + 1] != ':' || (header[i + 1] == ':' && header[i + 2] == ':')); ++i) {
        codes[byte][j++] = header[i] - '0';
        codesLens[headerSortedIndex] = j;
      }
      headerSortedIndex++;
    }
  }
}

void readHeader(FILE* fp, char* header) {
  for (int i = 0; i < BYTES_COUNT * CODE_SIZE; ++i) {
    header[i] = (char)getc(fp);
    if (header[i - 1] == '\n' && header[i] == '\n') {
      header[i - 1] = '\0';
      header[i] = '\0';
      break;
    }
  }
}

void getUncompressedFileSize(FILE* fp, FILE_INFO* fileInfo) {
  char fileSizeString[15] = {0};
  char* errorPtr;
  fscanf(fp, "%s\n", fileSizeString); //  get archive file size in bits
  fileInfo->sizeUncompressed = strtoll(fileSizeString, &errorPtr, 16);
  fscanf(fp, "%s\n", fileInfo->filename); // get archive filename
  fileInfo->filenameLen = strlen(fileInfo->filename);
}

void decode(const char* archiveFilename) {
  printf("Start decode\n");
  CODES_AS_TREE* root;
  FILE_INFO fileInfo = {0};
  FILE* final;
  clock_t startTime, endTime;
  char header[BYTES_COUNT * CODE_SIZE] = {0};
  char codes[BYTES_COUNT][CODE_SIZE] = {0}; // [-1:1]
  unsigned char headerSorted[BYTES_COUNT] = {0};
  char outputFileName[FILENAME_PATH_LEN] = {0};
  int codesLens[CODE_SIZE] = {0};
  int ansIndex = 0;
  startTime = clock();
  memset(codes, -1, BYTES_COUNT*CODE_SIZE*sizeof(char));
  getFilePath(FILENAME_PATH_LEN, outputFileName, archiveFilename);
  unsigned long fileSize = getFileSize(archiveFilename);
  printLog("decode -> get file size: %lu bytes -> DONE.\n", fileSize);
  final = fopen(archiveFilename, "rb");
  readHeader(final, header);
  printLog("decode -> read header -> DONE.\n");
  createCodeTable(codes, header, headerSorted, codesLens, &fileSize);
  printLog("decode -> create a table -> DONE.\n");
  root = createBinTree(codes, headerSorted, codesLens);
  printLog("decode -> create a bin tree -> DONE.\n");
  getUncompressedFileSize(final, &fileInfo);
  printLog("decode -> get decodeFileSizeBytes: %ld -> DONE.\n", fileInfo.sizeUncompressed);
  printLog("decode -> get decodeFileName: %s -> DONE.\n", fileInfo.filename);
  fileSize -= fileInfo.filenameLen;
  fileSize -= 5; //?
  char* ans = calloc(ANSWER_BUFFER_SIZE, sizeof(*ans));
  int readIndex = 0;
  unsigned char* buffCode = calloc(DECODE_BUFF_SIZE, sizeof(*buffCode));
  unsigned long decodeBits = 0;
  unsigned int onePercentOfFile = fileInfo.sizeUncompressed / 100;
  unsigned long fileCurrentLen = fileSize;
  unsigned char* contentBuff = calloc(DECODE_BUFF_SIZE / BIT8, sizeof(*contentBuff));
  fread(contentBuff, sizeof(unsigned char), DECODE_BUFF_SIZE / BIT8, final);
  for (int i = 0; i < DECODE_BUFF_SIZE / BIT8 && i < fileSize; ++i) {
    for (int j = 0; j < BIT8; ++j, --fileCurrentLen) {
      buffCode[BIT8 * i + j] = (contentBuff[i] >> j) & 1;
    }
  }
  printLog("decode -> first 80k read -> DONE.\n");
  strncat(outputFileName, fileInfo.filename, (sizeof outputFileName) - fileInfo.filenameLen - 1);
  FILE* fp = fopen(outputFileName, "wb");
  clock_t loopStart, loopEnd;
  loopStart = clock();
  while (decodeBits != fileInfo.sizeUncompressed) { // start
#ifdef ENABLE_PROGRESS
    if (onePercentOfFile && !(decodeBits % onePercentOfFile)) {
      loopEnd = clock();
      printProgress(fileInfo.sizeUncompressed, decodeBits, loopEnd, loopStart);
      loopStart = clock();
    }
#endif
    ans[ansIndex++] = findAnswer(root, buffCode, &readIndex);
    if (decodeBits % ANSWER_BUFFER_SIZE == 0 || decodeBits == fileInfo.sizeUncompressed - 1) {
      fwrite(ans, 1, ansIndex, fp);
      ansIndex = 0;
      memset(ans, 0, (ANSWER_BUFFER_SIZE) * sizeof(*ans));
    }
    if (readIndex > (DECODE_BUFF_SIZE - CODE_SIZE) && fileCurrentLen) {
      prepareByteBuffer(buffCode, final, &readIndex, DECODE_BUFF_SIZE, &fileCurrentLen);
    }
    decodeBits++;
  }
  fclose(fp);
  fclose(final);
  freeBinTree(root);
  free(root);
  free(buffCode);
  free(contentBuff);
  free(ans);
  endTime = clock();
  printf("\ndecode time: %.2lf sec.\n", (double)(endTime - startTime) / (CLOCKS_PER_SEC));
}

void prepareByteBuffer(unsigned char* buffCode, FILE* fp, int* iRead, int arrSize, unsigned long* fileLen) {
  unsigned char* contentBuff = calloc(DECODE_BUFF_SIZE / 8, sizeof(*contentBuff));
  unsigned char trashCount = 0;
  unsigned char magicNumber = 31; // ?
  unsigned int readCount;
  for (int i = arrSize - 1; buffCode[i] == 255 && i > arrSize - 256; --i) trashCount++;
  memcpy(buffCode, buffCode+(*iRead), (arrSize - *iRead - trashCount)*sizeof(*buffCode));
  *iRead = arrSize - *iRead - trashCount;
  memset(buffCode+(arrSize-magicNumber), UINT8_MAX, magicNumber*sizeof(*buffCode));
  readCount = (arrSize - *iRead) / 8;
  fread(contentBuff, sizeof(*contentBuff), readCount, fp);
  for (unsigned int i = 0; i < readCount; ++i) { //~10k
    for (unsigned char j = 0; j < BIT8; ++j) {
      buffCode[*iRead + BIT8 * i + j] = ((contentBuff[i] >> j) & 1);
    }
  }
  *fileLen -= readCount;
  *iRead = 0;
  free(contentBuff);
}

char findAnswer(CODES_AS_TREE* root, const unsigned char* arrayLen, int* readIndex) {
  CODES_AS_TREE* currentNode = root;
  for (; !currentNode->is_symbol; ++*readIndex)
    currentNode = arrayLen[*readIndex] == 0 ? currentNode->left : currentNode->right;
  return currentNode->symbol;
}

void Add2Tree(CODES_AS_TREE* root, int arrayLen, int deepIndex, char* codes, char value) {
  CODES_AS_TREE* currentNode = root;
  for (int i = deepIndex; i < arrayLen; ++i) {
    if (codes[i] == 0) {
      if (currentNode->left == NULL) currentNode->left = (CODES_AS_TREE*)calloc(1, sizeof(CODES_AS_TREE));
      currentNode = currentNode->left;
    } else {
      if (currentNode->right == NULL) currentNode->right = (CODES_AS_TREE*)calloc(1, sizeof(CODES_AS_TREE));
      currentNode = currentNode->right;
    }
  }
  currentNode->symbol = value;
  currentNode->is_symbol = true;
}

void freeBinTree(CODES_AS_TREE* root) {
  if (root->left != NULL) {
    freeBinTree(root->left);
    free(root->left);
  }
  if (root->right != NULL) {
    freeBinTree(root->right);
    free(root->right);
  }
}
