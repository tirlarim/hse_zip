// DEPRECATED


//#include <string.h>
//#include <stdio.h>
//#include <time.h>
//#include "tree_list/tree_list.h"
//#include "fileWorker/filePrepare.h"
//#include "utils/printColors.h"
//#include "tree_list/union.h"
//
//void decode(char* archiveFilename);
//void printTreeCodes(const NODE* init);
//void prepareBytesBuffer(int* buffCode, FILE* fp, int* iRead, int arrSize, unsigned long* fileLen);
//char findAnswer(CODES_AS_TREE* root, const int* arrayLen, long long* offset, int readIndex);
//void fillArrMinusOne(int* arr);
//CODES_AS_TREE* Add2Tree(CODES_AS_TREE* root, int arrayLen, int deepIndex, int* arr, char value);
//
//int main(int argc, char* argv[]) {
//  if (argc < 3) {printf("wrong paths\n");exit(1);}
//  char fileNameInput[100], fileNameOutput[100];
//  strcpy(fileNameInput, argv[1]); strcpy(fileNameOutput, argv[2]);
//  printf("working on: %s\n", fileNameInput);
//  decode(fileNameOutput);
//  if (__APPLE__) {
//    printf(ANSI_COLOR_YELLOW"check sha 256 sum:"ANSI_COLOR_RESET" ");
//    checkFileHash(fileNameInput, fileNameOutput);
//    checkFileSize(fileNameInput, fileNameOutput);
//  }
//  return 0;
//}
//
//void printProgress(double percentage, unsigned long long sec) {
//  int val = (int) (percentage * 100);
//  int leftPad = (int) (percentage * PBWIDTH);
//  int rightPad = PBWIDTH - leftPad;
//  sec = percentage ? sec : 0;
//  printf("\r%3d%% [%.*s%*s] est. time: ~ ", val, leftPad, PBSTR, rightPad, "");
//  if (sec >= 604800) {
//    printf("%llu weeks %lld days %lld hours %lld minutes %lld sec", sec/604800, sec/604800/86400, sec/604800/86400/3600, sec/604800/86400/3600/60, sec%60);
//  } else if (sec >= 86400) {
//    printf("%lld days %lld hours %lld minutes %lld sec", sec/86400, sec/86400/3600, sec/86400/3600/60, sec%60);
//  } else if (sec >= 3600) {
//    printf("%lld hours %lld minutes %lld sec", sec/3600, sec/3600/60, sec%60);
//  } else if (sec >= 60) {
//    printf("%lld minutes %lld sec", sec/60, sec%60);
//  } else if (sec < 60) {
//    printf("%lld sec", sec%60);
//  }
//  fflush(stdout);
//}
//
//void decode(char* fileNameOutput) {
//  clock_t startTime, endTime;
//  startTime = clock();
//  char header[BYTES_COUNT*CODE_SIZE] = {0};
//  unsigned char headerSorted[BYTES_COUNT] = {0}; int headerSortedIndex = 0;
//  char outputFileName[1000] = "../testDataOutput/";
//  int ansIndex = 0;
//  unsigned long fileNameLength;
//  char decodeFileName[1000] = {0};
//  unsigned long long decodeFileSizeBytes = 0;
//  int codes[BYTES_COUNT][CODE_SIZE] = {0};
//  for (int i = 0; i < CODE_SIZE; ++i) {
//    for (int j = 0; j < CODE_SIZE; ++j) {
//      codes[i][j] = -1;
//    }
//  }
////  get file size
//  FILE* output = fopen(fileNameOutput, "rb");
//  if (!output) exit(2);
//  fseek(output, 0, SEEK_END);
//  unsigned long length = ftell(output);
//  fseek(output, 0, SEEK_SET);
//  fclose(output);
////  read header
//  FILE* final = fopen(fileNameOutput, "rb");
//  for (int i = 0; i < BYTES_COUNT * CODE_SIZE; ++i) {
//    header[i] = (char)getc(final);
////    printf("%c", header[i]);
//    if (header[i-1] == '\n' && header[i] == '\n') {
//      header[i-1] = '\0'; header[i] = '\0';
//      break;
//    }
//  }
////  create a table
//  for (int i = 1; header[i] != '\0'; ++i) {
//    unsigned char byte;
//    if (header[i-1] == ':' && (header[i] == 48 || header[i] == 49)) {
//      byte = header[i-2];
//      headerSorted[headerSortedIndex++] = byte;
//      for (int j = 0;(header[i] == 48 || header[i] == 49) && (header[i+1] != ':' || (header[i+1] == ':' && header[i+2] == ':')); ++i) {
//        codes[byte][j++] = header[i] - '0';
//      }
//    }
//    length--;
//  }
//  CODES_AS_TREE* root = NULL;
//  root = (CODES_AS_TREE*)malloc(sizeof(CODES_AS_TREE));
//  root->right=NULL;
//  root->left=NULL;
//  int arrLen[256];
//  memset(arrLen, 0, sizeof(arrLen));
//  for (int i = 0; i < 256; ++i) {
//    for (int j = 0; codes[(int)headerSorted[i]][j] != -1 && j < 256; ++j) {
//      arrLen[i] = j+1;
//    }
//  }
//  for (int i = 0; i < 256; ++i) {
//    int arrCurrentLen = 0;
//    if (codes[(int)headerSorted[i]][0] != -1) {
//      root = Add2Tree(root, arrLen[i], arrCurrentLen, codes[(int)headerSorted[i]], (char)headerSorted[i]);
//    }
//  }
//  length-=5;
//  fscanf(final, "%lld\n",  &decodeFileSizeBytes); //  get bits count
//  fscanf(final, "%s\n",  decodeFileName); //  get bits count
//  fileNameLength = strlen(decodeFileName);
//  length -= fileNameLength;
//  char ans[1000+1];
//  printf("start decode\n");
//  int readIndex = 0;
//  int arrSize = 80000; // this isn't limit. we can create arr bigger
//  int buffCode[8*10000] = {0}; //arrSize = buffCode.length
//  long long lastOffset = 0;
//  unsigned long long decodeBits = 0;
//  unsigned int onePercentOfFile = decodeFileSizeBytes/100;
//  unsigned long fileCurrentLen = length;
//  BIT_TO_CHAR symbol;
//  fillArrMinusOne(buffCode);
//  for (int i = 0; i < arrSize/8 && i < length; ++i) {
//    symbol.character = (unsigned char)getc(final);
//    buffCode[8*i+0] = (int)symbol.mbit.b1;
//    buffCode[8*i+1] = (int)symbol.mbit.b2;
//    buffCode[8*i+2] = (int)symbol.mbit.b3;
//    buffCode[8*i+3] = (int)symbol.mbit.b4;
//    buffCode[8*i+4] = (int)symbol.mbit.b5;
//    buffCode[8*i+5] = (int)symbol.mbit.b6;
//    buffCode[8*i+6] = (int)symbol.mbit.b7;
//    buffCode[8*i+7] = (int)symbol.mbit.b8;
//    --fileCurrentLen;
//  }
//  strncat(outputFileName, decodeFileName, sizeof(outputFileName) - fileNameLength - 1);
//  FILE *fp = fopen(outputFileName, "wb" );
//  clock_t loopStart, loopEnd;
//  loopStart = clock();
//  while (decodeBits < decodeFileSizeBytes) { ////////////// start
//    if (decodeBits % onePercentOfFile == 0 && onePercentOfFile != 0) {
//      loopEnd = clock();
//      double progress = ((double)decodeBits / (double)decodeFileSizeBytes) + 0.01;
//      printProgress(progress,(unsigned long long)(((double)(loopEnd - loopStart) / (CLOCKS_PER_SEC))*(((double)1-progress)*100)));
//      loopStart = clock();
//    }
//    lastOffset = 0;
//    ans[ansIndex++] = findAnswer(root, buffCode, &lastOffset, readIndex);
//    readIndex+=(int)lastOffset;
//    if (decodeBits % 1000 == 0 || decodeBits == decodeFileSizeBytes - 1) {
//      fwrite(ans , 1, ansIndex, fp);
//      ansIndex = 0;
//      memset(ans, 0, sizeof(ans));
//    }
//    if (readIndex>(arrSize-256) && fileCurrentLen) {
//      prepareBytesBuffer(buffCode, final, &readIndex, arrSize, &fileCurrentLen);
//    }
//    decodeBits++;
//  }
//  fclose(fp);
//  endTime = clock();
//  printf("\n");
//  printf("decode time: %.2lf sec.\n", (double)(endTime - startTime) / (CLOCKS_PER_SEC));
//}
//
//void prepareBytesBuffer(int* buffCode, FILE* fp, int* iRead, int arrSize, unsigned long* fileLen) {
//  int trashCount = 0;
//  for (int i = arrSize-1; buffCode[i] == -1 && i > arrSize-256; --i) {
//    trashCount++; // ~10-15 iterations
//  }
//  for (int i = *iRead; i < arrSize && buffCode[i] != -1; ++i) {
//    buffCode[i-*iRead] = buffCode[i]; //~256 iterations
//  }
//  *iRead = arrSize-*iRead-trashCount;
//  for (int i = arrSize-1; i > arrSize-30; --i) {
//    buffCode[i] = -1; //~30 iterations
//  }
//  BIT_TO_CHAR symbol;
//  for (int i = 0; i < (arrSize - *iRead)/8; ++i) {
//    symbol.character = (unsigned char)getc(fp); // fix it
//    buffCode[*iRead+8*i+0] = (int)symbol.mbit.b1;
//    buffCode[*iRead+8*i+1] = (int)symbol.mbit.b2;
//    buffCode[*iRead+8*i+2] = (int)symbol.mbit.b3;
//    buffCode[*iRead+8*i+3] = (int)symbol.mbit.b4;
//    buffCode[*iRead+8*i+4] = (int)symbol.mbit.b5;
//    buffCode[*iRead+8*i+5] = (int)symbol.mbit.b6;
//    buffCode[*iRead+8*i+6] = (int)symbol.mbit.b7;
//    buffCode[*iRead+8*i+7] = (int)symbol.mbit.b8;
//    --*fileLen;
//  }
//  *iRead=0;
//}
//
//void fillArrMinusOne(int arr[256*2]) {
//  int arrLen = 256*2;
//  for (int i = 0; i < arrLen; ++i) {
//    arr[i] = -1;
//  }
//}
//
//char findAnswer(CODES_AS_TREE* root, const int* arrayLen, long long* offset, int readIndex) {
//  if (root->is_symbol==true) {
//    return root->symbol;
//  } else {
//    if (arrayLen[*offset+readIndex] == 1) {
//      ++(*offset);
//      return findAnswer(root->right, arrayLen, offset, readIndex);
//    } else {
//      ++(*offset);
//      return findAnswer(root->left, arrayLen, offset, readIndex);
//    }
//  }
//}
//
//
//CODES_AS_TREE* Add2Tree(CODES_AS_TREE* root, int arrayLen, int deepIndex, int* arr, char value) {
//  if (root == NULL) {
//    root = (CODES_AS_TREE*)malloc(sizeof(CODES_AS_TREE));
//  }
//  if (deepIndex == arrayLen) {
//    root->symbol = value;
//    root->is_symbol = true;
//  }
//  if (deepIndex != arrayLen && arr[deepIndex] == 0) {
//    root->left = Add2Tree(root->left, arrayLen, ++deepIndex, arr, value);
//    return root;
//  }
//  if (deepIndex != arrayLen && arr[deepIndex] == 1) {
//    root->right = Add2Tree(root->right, arrayLen, ++deepIndex, arr, value);
//    return root;
//  }
//  return root;
//}