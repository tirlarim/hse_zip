#include <string.h>
#include <stdio.h>
#include <time.h>
#include "tree_list/tree_list.h"
#include "fileWorker/filePrepare.h"
#include "utils/printColors.h"
#include "tree_list/tree_list.h"
#include "tree_list/union.h"

void decode(char* fileNameOutput);
void printTreeCodes(const NODE* init);
void prepareBytesBuffer(int* buffCode, FILE* fp, int* iRead, int arrSize, unsigned long* fileLen);
bool findAnswer(const int* bitsArr, const int symbolCodeArr[], long long* codeLen, int readIndex);
void fillArrMinusOne(int* arr);

int main(int argc, char* argv[]) {
  NODE* binTree = NULL;
  char fileNameInput[] = "../testDataInput/2mb.jpg",
          fileNameOutput[] = "../testDataOutput/answer.hse_zip";

  printf(ANSI_COLOR_YELLOW"check sha 256 sum:"ANSI_COLOR_RESET" ");
  decode(fileNameOutput);
  //checkFileHash(fileNameInput, fileNameOutput);
  checkFileSize(fileNameInput, fileNameOutput);
  return 0;
}

void printProgress(double percentage, unsigned long long sec) {
  int val = (int) (percentage * 100);
  int lpad = (int) (percentage * PBWIDTH);
  int rpad = PBWIDTH - lpad;
  printf("\r%3d%% [%.*s%*s] est. time: ~%llu sec.", val, lpad, PBSTR, rpad, "", sec);
  fflush(stdout);
}

void decode(char* fileNameOutput) {
  clock_t startTime, endTime;
  startTime = clock();
  char header[BYTES_COUNT*CODE_SIZE] = {0};
  unsigned char headerSorted[BYTES_COUNT] = {0}; int headerSortedIndex = 0;
  char outputFileName[1000] = "../testDataOutput/";
  int ansIndex = 0;
  unsigned long fileNameLength;
  char decodeFileName[1000] = {0};
  long long decodeFileSizeBytes = 0;
  int codes[BYTES_COUNT][CODE_SIZE] = {0};
  for (int i = 0; i < CODE_SIZE; ++i) {
    for (int j = 0; j < CODE_SIZE; ++j) {
      codes[i][j] = -1;
    }
  }
//  get file size
  FILE* output = fopen(fileNameOutput, "rb");
  if (!output) exit(2);
  fseek(output, 0, SEEK_END);
  unsigned long length = ftell(output);
  fseek(output, 0, SEEK_SET);
  fclose(output);
//  read header
  FILE* final = fopen(fileNameOutput, "rb");
  for (int i = 0; i < BYTES_COUNT * CODE_SIZE; ++i) {
    header[i] = (char)getc(final);
//    printf("%c", header[i]);
    if (header[i-1] == '\n' && header[i] == '\n') {
      header[i-1] = '\0'; header[i] = '\0';
      break;
    }
  }
//  create a table
  for (int i = 1; header[i] != '\0'; ++i) {
    unsigned char byte;
    if (header[i-1] == ':' && (header[i] == 48 || header[i] == 49)) {
      byte = header[i-2];
      headerSorted[headerSortedIndex++] = byte;
      for (int j = 0;(header[i] == 48 || header[i] == 49) && (header[i+1] != ':' || (header[i+1] == ':' && header[i+2] == ':')); ++i) {
        codes[byte][j++] = header[i] - '0';
      }
    }
    length--;
  }
  length-=5;
  fscanf(final, "%lld\n",  &decodeFileSizeBytes); //  get bits count
  printf("file size -> %lld\n", decodeFileSizeBytes);
  fscanf(final, "%s\n",  decodeFileName); //  get bits count
  printf("file name -> %s\n", decodeFileName);
  fileNameLength = strlen(decodeFileName);
  length -= fileNameLength;
  char ans[1000+1];
  printf("start decode\n");
  int readIndex = 0;
  int arrSize = 80000; // this isn't limit. we can create arr bigger
  int buffCode[8*10000] = {0}; //arrSize = buffCode.length
  long long lastOffset = 0;
  int a = 0;
  unsigned int onePercentOfFile = decodeFileSizeBytes/100;
  unsigned long fileCurrentLen = length;
  BIT_TO_CHAR symbol;
  fillArrMinusOne(buffCode);
  for (int i = 0; i < arrSize/8 && i < length; ++i) {
    symbol.character = (unsigned char)getc(final);
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
  strncat(outputFileName, decodeFileName, sizeof(outputFileName) - fileNameLength - 1);
  FILE *fp = fopen(outputFileName, "wb" );
  clock_t loopStart, loopEnd;
  loopStart = clock();
  while (a < decodeFileSizeBytes) { ////////////// start
    if (a % onePercentOfFile == 0 && onePercentOfFile != 0) {
      loopEnd = clock();
      double progress = ((double)a/(double)decodeFileSizeBytes)+0.01;
      printProgress(progress,(unsigned long long)(((double)(loopEnd - loopStart) / (CLOCKS_PER_SEC))*(((double)1-progress)*100)));
      loopStart = clock();
    }
    for (int i = 0; i < 256; ++i) {
      if (codes[(int)headerSorted[i]][0] != -1) {
        if (findAnswer(buffCode, codes[(int)headerSorted[i]], &lastOffset, readIndex)) {
          readIndex+=(int)lastOffset;
          ans[ansIndex++] = (char)headerSorted[i];
          if (a % 1000 == 0 || a == decodeFileSizeBytes-1) {
            fwrite(ans , 1, ansIndex, fp);
            ansIndex = 0;
            memset(ans, 0, sizeof(ans));
          }
          break;
        }
      }
    }
    if (readIndex>(arrSize-256) && fileCurrentLen) {
      prepareBytesBuffer(buffCode, final, &readIndex, arrSize, &fileCurrentLen);
    }
    a++;
  }
  fclose(fp);
  endTime = clock();
  printf("\n");
  printf("decode time: %.2lf sec.\n", (double)(endTime - startTime) / (CLOCKS_PER_SEC));
}

void prepareBytesBuffer(int* buffCode, FILE* fp, int* iRead, int arrSize, unsigned long* fileLen) {
  int trashCount = 0;
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
  BIT_TO_CHAR symbol;
  for (int i = 0; i < (arrSize - *iRead)/8; ++i) {
    symbol.character = (unsigned char)getc(fp); // fix it
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

void fillArrMinusOne(int arr[256*2]) {
  int arrLen = 256*2;
  for (int i = 0; i < arrLen; ++i) {
    arr[i] = -1;
  }
}

bool findAnswer(const int* bitsArr, const int symbolCodeArr[256], long long* codeLen, int readIndex) {
  for (int i = 0; symbolCodeArr[i] != -1 && i < 256 ; ++i) {
    if (symbolCodeArr[i] != bitsArr[readIndex+i]) {
      return false;
    }
    *codeLen = i+1;
  }
  return true;
}