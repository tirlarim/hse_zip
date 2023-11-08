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


void prepareBytesBuffer(int* buffCode, FILE* fp, int* iRead, int arrSize, unsigned long* fileLen);
char findAnswer(CODES_AS_TREE* root, const int* arrayLen, int* offset, int readIndex);
CODES_AS_TREE* Add2Tree(CODES_AS_TREE* root, int arrayLen, int deepIndex, int* arr, char value);
void freeTree(CODES_AS_TREE* root);

void decodeArchive(char* fileNameOutput) {
  decode(fileNameOutput);
  printLog("decode - DONE.\n");
}

void decode(char* archiveFilename) {
  printf("Start decode\n");
  clock_t startTime, endTime;
  startTime = clock();
  char header[BYTES_COUNT*CODE_SIZE] = {0};
  unsigned char headerSorted[BYTES_COUNT] = {0}; int headerSortedIndex = 0;
  char outputFileName[FILENAME_PATH_LEN] = {0};
  int ansIndex = 0;
  unsigned long fileNameLength;
  char decodeFileName[FILENAME_PATH_LEN] = {0};
  unsigned long long decodeFileSizeBytes = 0;
  int codes[BYTES_COUNT][CODE_SIZE] = {0};
  char *errorPtr;
  char fileSizeString[15] = {0};
  memset(header, '\0', sizeof header);
  memset(headerSorted, '\0', sizeof headerSortedIndex);
  memset(outputFileName, '\0', sizeof outputFileName);
  memset(decodeFileName, '\0', sizeof decodeFileSizeBytes);
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
    if (header[i-1] == '\n' && header[i] == '\n') {
      header[i-1] = '\0'; header[i] = '\0';
      break;
    }
  }
  printLog("decode -> read header -> DONE.\n");
//  create a table
  for (int i = 1; header[i] != '\0'; ++i, --length) {
    unsigned char byte;
    if (header[i-1] == ':' && (header[i] == 48 || header[i] == 49)) {
      byte = header[i-2];
      headerSorted[headerSortedIndex++] = byte;
      for (int j = 0; (header[i] == 48 || header[i] == 49) && (header[i+1] != ':' || (header[i+1] == ':' && header[i+2] == ':')); ++i) {
        codes[byte][j++] = header[i] - '0';
      }
    }
  }
  printLog("decode -> create a table -> DONE.\n");
  CODES_AS_TREE* root = NULL;
  root = (CODES_AS_TREE*)malloc(sizeof(CODES_AS_TREE));
  root->right=NULL;
  root->left=NULL;
  int arrLen[256];
  memset(arrLen, 0, 256*sizeof(int));
  for (int i = 0; i < 256; ++i) {
    for (int j = 0; codes[headerSorted[i]][j] != -1 && j < 256; ++j) {
      arrLen[i] = j+1;
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
  decodeFileSizeBytes = strtoll(fileSizeString, &errorPtr, 16);
  fscanf(final, "%s\n",  decodeFileName); // get archive filename
  if (PRINTF_DEBUG) {
    if (__APPLE__) printf(ANSI_COLOR_YELLOW);
    printCurrentTime();
    printf("decode -> get decodeFileSizeBytes: %lld -> DONE.\n", decodeFileSizeBytes);
    printCurrentTime();
    printf("decode -> get decodeFileName: %s -> DONE.\n", decodeFileName);
    if (__APPLE__) printf(ANSI_COLOR_RESET);
  }
  fileNameLength = strlen(decodeFileName);
  length -= fileNameLength;
  length -= 5; //?
  char ans[1000+1];
  int readIndex = 0;
  int buffCode[DECODE_BUFF_SIZE] = {0}; //arrSize = buffCode.length
  int lastOffset = 0;
  unsigned long long decodeBits = 0;
  unsigned int onePercentOfFile = decodeFileSizeBytes/100;
  unsigned long fileCurrentLen = length;
  unsigned char contentBuff[DECODE_BUFF_SIZE/8];
  BIT_TO_CHAR symbol;
  memset(buffCode, 0, DECODE_BUFF_SIZE*sizeof(int));
  memset(contentBuff, 0, DECODE_BUFF_SIZE/8*sizeof(unsigned char));
  fread(contentBuff, sizeof(unsigned char), DECODE_BUFF_SIZE/8, final);
  for (int i = 0; i < DECODE_BUFF_SIZE/8 && i < length; ++i) {
    symbol.character = contentBuff[i];
    buffCode[8*i+0] = (int)symbol.mbit.b1;
    buffCode[8*i+1] = (int)symbol.mbit.b2;
    buffCode[8*i+2] = (int)symbol.mbit.b3;
    buffCode[8*i+3] = (int)symbol.mbit.b4;
    buffCode[8*i+4] = (int)symbol.mbit.b5;
    buffCode[8*i+5] = (int)symbol.mbit.b6;
    buffCode[8*i+6] = (int)symbol.mbit.b7;
    buffCode[8*i+7] = (int)symbol.mbit.b8;
    --fileCurrentLen;
  }
  if (PRINTF_DEBUG) {printCurrentTime(); printf("decode -> first 80k read -> DONE.\n");}
  strncat(outputFileName, decodeFileName, (sizeof outputFileName) - fileNameLength - 1);
  FILE *fp = fopen(outputFileName, "wb");
  clock_t loopStart, loopEnd;
  loopStart = clock();
  while (decodeBits < decodeFileSizeBytes) { // start
    if (onePercentOfFile != 0 && decodeBits % onePercentOfFile == 0) {
      loopEnd = clock();
      double progress = ((double)decodeBits / (double)decodeFileSizeBytes) + 0.01;
      printProgress(progress,(long long)(((double)(loopEnd - loopStart) / (CLOCKS_PER_SEC))*(((double)1-progress)*100)));
      loopStart = clock();
    }
    lastOffset = 0;
    ans[ansIndex++] = findAnswer(root, buffCode, &lastOffset, readIndex);
    readIndex+=lastOffset;
    if (decodeBits % 1000 == 0 || decodeBits == decodeFileSizeBytes - 1) {
      fwrite(ans , 1, ansIndex, fp);
      ansIndex = 0;
      memset(ans, 0, sizeof(ans));
    }
    if (readIndex>(DECODE_BUFF_SIZE-256) && fileCurrentLen) {
      prepareBytesBuffer(buffCode, final, &readIndex, DECODE_BUFF_SIZE, &fileCurrentLen);
    }
    decodeBits++;
  }
  fclose(fp);
  fclose(final);
  freeTree(root);
  free(root);
  endTime = clock();
  printf("\n");
  printf("decode time: %.2lf sec.\n", (double)(endTime - startTime) / (CLOCKS_PER_SEC));
}

void prepareBytesBuffer(int* buffCode, FILE* fp, int* iRead, int arrSize, unsigned long* fileLen) {
  int trashCount = 0;
  unsigned char contentBuff[10000];
  memset(contentBuff, 0, 10000*sizeof(unsigned char));
  BIT_TO_CHAR symbol;
  for (int i = arrSize-1; buffCode[i] == -1 && i > arrSize-256; --i) {
    trashCount++; // ~10-15 iterations
  }
  for (int i = *iRead; i < arrSize && buffCode[i] != -1; ++i) {
    buffCode[i-*iRead] = buffCode[i]; //~256 iterations
  }
  *iRead = arrSize-*iRead-trashCount;
  for (int i = arrSize-1; i > arrSize-30; --i) {
    buffCode[i] = -1; //~30 iterations
  }
  fread(contentBuff, sizeof(unsigned char), (arrSize - *iRead)/8, fp);
  for (int i = 0; i < (arrSize - *iRead)/8; ++i) { //~10k
    symbol.character = contentBuff[i];
    buffCode[*iRead+8*i+0] = (int)symbol.mbit.b1;
    buffCode[*iRead+8*i+1] = (int)symbol.mbit.b2;
    buffCode[*iRead+8*i+2] = (int)symbol.mbit.b3;
    buffCode[*iRead+8*i+3] = (int)symbol.mbit.b4;
    buffCode[*iRead+8*i+4] = (int)symbol.mbit.b5;
    buffCode[*iRead+8*i+5] = (int)symbol.mbit.b6;
    buffCode[*iRead+8*i+6] = (int)symbol.mbit.b7;
    buffCode[*iRead+8*i+7] = (int)symbol.mbit.b8;
    --*fileLen;
  }
  *iRead=0;
}

char findAnswer(CODES_AS_TREE* root, const int* arrayLen, int* offset, int readIndex) {
  if (root->is_symbol==false) {
    if (arrayLen[*offset+readIndex] == 1) {
      ++(*offset);
      return findAnswer(root->right, arrayLen, offset, readIndex);
    } else {
      ++(*offset);
      return findAnswer(root->left, arrayLen, offset, readIndex);
    }
  } else {
    return root->symbol;
  }
}

//root
CODES_AS_TREE* Add2Tree(CODES_AS_TREE* root, int arrayLen, int deepIndex, int* arr, char value) {
  if (root == NULL) {
    root = (CODES_AS_TREE*)malloc(sizeof(CODES_AS_TREE));
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

void freeTree(CODES_AS_TREE* root) {
  if (root->left != NULL) {
    freeTree(root->left);
    free(root->left);
  }
  if (root->right != NULL) {
    freeTree(root->right);
    free(root->right);
  }
}
