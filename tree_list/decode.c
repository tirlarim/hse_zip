#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "tree_list.h"
#include "decode.h"
#include "union.h"
#include "../utils/time.h"
#include "../utils/utils.h"
#include "../utils/printColors.h"

void decode(const char* archiveFilename);
void prepareByteBuffer(unsigned char* buffCode, FILE* fp, int* iRead, int arrSize, unsigned long* fileLen);
char findAnswer(CODES_AS_TREE* root, const unsigned char* arrayLen, int* readIndex);
CODES_AS_TREE* Add2Tree(CODES_AS_TREE* root, int arrayLen, int deepIndex, int* arr, char value);
void freeBinTree(CODES_AS_TREE* root);

void decodeArchive(char* fileNameOutput) {
  decode(fileNameOutput);
  printLog("decode - DONE.\n");
}

void decode(const char* archiveFilename) {
  printf("Start decode\n");
  clock_t startTime, endTime;
  startTime = clock();
  char header[BYTES_COUNT * CODE_SIZE] = {0};
  unsigned char headerSorted[BYTES_COUNT] = {0};
  int headerSortedIndex = 0;
  char outputFileName[FILENAME_PATH_LEN] = {0};
  int ansIndex = 0;
  unsigned long fileNameLength;
  char decodeFileName[FILENAME_PATH_LEN] = {0};
  unsigned long fileSizeBits = 0;
  int codes[BYTES_COUNT][CODE_SIZE] = {0};
  char* errorPtr;
  char fileSizeString[15] = {0};
  memset(header, '\0', sizeof header);
  memset(headerSorted, '\0', sizeof headerSorted);
  memset(outputFileName, '\0', sizeof outputFileName);
  memset(decodeFileName, '\0', sizeof decodeFileName);
  memset(fileSizeString, '\0', sizeof fileSizeString);
  for (int i = 0; i < CODE_SIZE; ++i) {
    for (int j = 0; j < CODE_SIZE; ++j) {
      codes[i][j] = -1;
    }
  }
  getFilePath(FILENAME_PATH_LEN, outputFileName, archiveFilename);
//  get file size
  FILE* output = fopen(archiveFilename, "rb");
  if (!output) exit(2);
  fseek(output, 0, SEEK_END);
  unsigned long long length = ftell(output);
  fseek(output, 0, SEEK_SET);
  fclose(output);
  if (PRINTF_DEBUG) {
    if (__APPLE__) printf(ANSI_COLOR_YELLOW);
    printCurrentTime();
    printf("decode -> get file size: %llu bytes -> DONE.\n", length);
    if (__APPLE__) printf(ANSI_COLOR_RESET);
  }
//  read header
  FILE* final = fopen(archiveFilename, "rb");
  for (int i = 0; i < BYTES_COUNT * CODE_SIZE; ++i) {
    header[i] = (char)getc(final);
    if (header[i - 1] == '\n' && header[i] == '\n') {
      header[i - 1] = '\0';
      header[i] = '\0';
      break;
    }
  }
  printLog("decode -> read header -> DONE.\n");
//  create a table
  for (int i = 1; header[i] != '\0'; ++i, --length) {
    unsigned char byte;
    if (header[i - 1] == ':' && (header[i] == 48 || header[i] == 49)) {
      byte = header[i - 2];
      headerSorted[headerSortedIndex++] = byte;
      for (int j = 0; (header[i] == 48 || header[i] == 49) &&
                      (header[i + 1] != ':' || (header[i + 1] == ':' && header[i + 2] == ':')); ++i) {
        codes[byte][j++] = header[i] - '0';
      }
    }
  }
  printLog("decode -> create a table -> DONE.\n");
  CODES_AS_TREE* root = (CODES_AS_TREE*)malloc(sizeof(*root));
  root->right = NULL;
  root->left = NULL;
  int arrLen[256];
  memset(arrLen, 0, 256 * sizeof(int));
  for (int i = 0; i < 256; ++i) {
    for (int j = 0; codes[headerSorted[i]][j] != -1 && j < 256; ++j) {
      arrLen[i] = j + 1;
    }
  }
  for (int i = 0; i < 256; ++i) {
    int arrCurrentLen = 0;
    if (codes[(int)headerSorted[i]][0] != -1) {
      root = Add2Tree(root, arrLen[i], arrCurrentLen, codes[(int)headerSorted[i]], (char)headerSorted[i]);
    }
  }
  printLog("decode -> create a bin tree -> DONE.\n");
  fscanf(final, "%s\n", fileSizeString); //  get archive file size in bits
  fileSizeBits = strtoll(fileSizeString, &errorPtr, 16);
  fscanf(final, "%s\n", decodeFileName); // get archive filename
  if (PRINTF_DEBUG) {
    if (__APPLE__) printf(ANSI_COLOR_YELLOW);
    printCurrentTime();
    printf("decode -> get decodeFileSizeBytes: %ld -> DONE.\n", fileSizeBits);
    printCurrentTime();
    printf("decode -> get decodeFileName: %s -> DONE.\n", decodeFileName);
    if (__APPLE__) printf(ANSI_COLOR_RESET);
  }
  fileNameLength = strlen(decodeFileName);
  length -= fileNameLength;
  length -= 5; //?
  char* ans = calloc(ANSWER_BUFFER_SIZE, sizeof(*ans));
  int readIndex = 0;
  unsigned char* buffCode = calloc(DECODE_BUFF_SIZE, sizeof(*buffCode));
  unsigned long decodeBits = 0;
  unsigned int onePercentOfFile = fileSizeBits / 100;
  unsigned long fileCurrentLen = length;
  unsigned char* contentBuff = calloc(DECODE_BUFF_SIZE / 8, sizeof(*contentBuff));
  fread(contentBuff, sizeof(unsigned char), DECODE_BUFF_SIZE / 8, final);
  for (int i = 0; i < DECODE_BUFF_SIZE / 8 && i < length; ++i) {
    for (int j = 0; j < BIT8; ++j, --fileCurrentLen) {
      buffCode[BIT8 * i + j] = (contentBuff[i] >> j) & 1;
    }
  }
  if (PRINTF_DEBUG) {
    printCurrentTime();
    printf("decode -> first 80k read -> DONE.\n");
  }
  strncat(outputFileName, decodeFileName, (sizeof outputFileName) - fileNameLength - 1);
  FILE* fp = fopen(outputFileName, "wb");
  clock_t loopStart, loopEnd;
  loopStart = clock();
  while (decodeBits != fileSizeBits) { // start
#ifdef ENABLE_PROGRESS
    if (onePercentOfFile && !(decodeBits % onePercentOfFile)) {
      loopEnd = clock();
      printProgress(fileSizeBits, decodeBits, loopEnd, loopStart);
      loopStart = clock();
    }
#endif
    ans[ansIndex++] = findAnswer(root, buffCode, &readIndex);
    if (decodeBits % ANSWER_BUFFER_SIZE == 0 || decodeBits == fileSizeBits - 1) {
      fwrite(ans, 1, ansIndex, fp);
      ansIndex = 0;
      memset(ans, 0, (ANSWER_BUFFER_SIZE) * sizeof(*ans));
    }
    if (readIndex > (DECODE_BUFF_SIZE - 256) && fileCurrentLen) {
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
  printf("\n");
  printf("decode time: %.2lf sec.\n", (double)(endTime - startTime) / (CLOCKS_PER_SEC));
}

void prepareByteBuffer(unsigned char* buffCode, FILE* fp, int* iRead, int arrSize, unsigned long* fileLen) {
  unsigned char* contentBuff = calloc(DECODE_BUFF_SIZE / 8, sizeof(*contentBuff));
  unsigned char trashCount = 0;
  unsigned char magicNumber = 31;
  unsigned int readCount;
  for (int i = arrSize - 1; buffCode[i] == 255 && i > arrSize - 256; --i) {
    trashCount++; // ~10-15 iterations
  }
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

CODES_AS_TREE* Add2Tree(CODES_AS_TREE* root, int arrayLen, int deepIndex, int* arr, char value) {
  if (root == NULL) {
    root = (CODES_AS_TREE*)calloc(1, sizeof(CODES_AS_TREE));
  }
  if (deepIndex == arrayLen) {
    root->symbol = value;
    root->is_symbol = true;
  }
  if (deepIndex != arrayLen && arr[deepIndex] == 0) {
    root->left = Add2Tree(root->left, arrayLen, ++deepIndex, arr, value);
    return root;
  }
  if (deepIndex != arrayLen && arr[deepIndex] == 1) {
    root->right = Add2Tree(root->right, arrayLen, ++deepIndex, arr, value);
    return root;
  }
  return root;
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
