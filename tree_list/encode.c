#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "tree_list.h"
#include "union.h"
#include "encode.h"
#include "../utils/time.h"
#include "../utils/utils.h"
#include "../utils/printColors.h"

void get_chars_frequency(char* filename, unsigned long long* freq_arr, unsigned long long* length);
void make_list(NODE** init, unsigned long long* freq_arr);
void make_tree(NODE** init);
void archive(char* input_filename, char* output_filename, unsigned long long length, NODE* init);
void create_codes(NODE* init, long long level, char* temp_code);
void freeTree(NODE* root);

void encodeArchive(char* fileNameInput, char* fileNameOutput) {
  NODE* pBinTree = NULL;
  clock_t startTime, endTime;
  unsigned long long length = 0;
  unsigned long long freq[SYMBOLS_COUNT] = {0};
  char temp_code[CODE_SIZE] = {0};
  startTime = clock();
  get_chars_frequency(fileNameInput, freq, &length);
  printLog("archive -> get_chars_frequency - DONE.\n");
  make_list(&pBinTree, freq);
  printLog("archive -> make_list - DONE.\n");
  make_tree(&pBinTree);
  printLog("archive -> make_tree - DONE.\n");
  create_codes(pBinTree, 0, temp_code);
  printLog("archive -> create_codes - DONE.\n");
  archive(fileNameInput, fileNameOutput, length, pBinTree); //take codes from buffer.txt and unite them
  printLog("archive - DONE.\n");
  freeTree(pBinTree);
  free(pBinTree);
  endTime = clock();
  printf("archive time: %.2lf sec.\n", (float)(endTime - startTime) / (CLOCKS_PER_SEC));
}

void symmetric(NODE* init, FILE* file, TRIPLE* arr) {
  if (init) {
    symmetric(init->left, file, arr);
    if (init->is_symbol) {
      arr[(int)init->symbol].symbol = init->symbol; //array for print symbols in frequency order
      arr[(int)init->symbol].freq = init->freq;
      strcpy(arr[(int)init->symbol].code, init->code);
    }
    symmetric(init->right, file, arr);
  }
}

void get_chars_frequency(char filename[], unsigned long long* freq_arr, unsigned long long* length) {
  FILE* input = fopen(filename, "rb");
  if (!input) {
    perror("get_chars_frequency -> Unable to open file");
    exit(2);
  }
  fseek(input, 0, SEEK_END);
  *length = ftell(input);
  fseek(input, 0, SEEK_SET);
  if (PRINTF_DEBUG) {
    if (__APPLE__) printf(ANSI_COLOR_YELLOW);
    printCurrentTime();
    printf("archive -> get file size: %llu - DONE.\n", *length);
    if (__APPLE__) printf(ANSI_COLOR_RESET);
  }
  unsigned char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE * sizeof(*buffer));
  for (int i = 0; i < *length / BUFFER_SIZE; ++i) {
    unsigned long long bytes_read = fread(buffer, sizeof(*buffer), BUFFER_SIZE, input);
    for (long long j = 0; j < bytes_read; ++j) {
      ++freq_arr[buffer[j]];
    }
  }
  fclose(input);
}

void create_codes(NODE* init, long long level, char* temp_code) {
  if (init) {
    if (init->is_symbol) {
      if (!level && !init->next) {
        init->code[0] = '1';
        init->code[1] = '\0';
        return;
      }
      temp_code[level] = '\0';
      strcpy(init->code, temp_code);
    }
    temp_code[level] = '0';
    create_codes(init->left, level + 1, temp_code);
    temp_code[level] = '1';
    create_codes(init->right, level + 1, temp_code);
  }
}

void find_and_copy_code(NODE* init, char** code_array, unsigned char symbol, unsigned char* symbolLen) {
  if (init) {
    if (init->is_symbol && init->symbol == symbol) {
      *symbolLen = strlen(init->code);
      strcpy(code_array[symbol], init->code);
      return;
    }
    find_and_copy_code(init->left, code_array, symbol, symbolLen);
    find_and_copy_code(init->right, code_array, symbol, symbolLen);
  }
}

void print_tree_on_side(const NODE* init, long long level) {
  if (init) {
    print_tree_on_side(init->right, level + 1);
    for (long long i = 0; i < level; i++) {
      printf("\t");
    }
    printf("%3d", init->freq);
    if (init->is_symbol)
      printf(":%c\n", init->symbol);
    else {
      printf("\n");
    }
    print_tree_on_side(init->left, level + 1);
  }
}

void printTreeCodes(const NODE* init) {
  if (init) {
    printTreeCodes(init->right);
    if (init->is_symbol) {
      printf("Symbol: '%c' \tfrequency: %d \tcode: ", init->symbol, init->freq);
      for (long long i = 0; i < CODE_SIZE; ++i) {
        printf("%d", init->code[i]);
      }
      printf("\n");
    }
    printTreeCodes(init->left);
  }
}

void freeTree(NODE* root) {
  if (root->right) {
    freeTree(root->right);
    free(root->right);
  }
  if (root->left) {
    freeTree(root->left);
    free(root->left);
  }
}

void add_to_list(NODE** init, unsigned long long freq, unsigned char symbol, NODE* branch) {
  NODE* pNode = NULL;
  while (*init) {
    if ((*init)->freq > freq) {
      break;
    }
    init = &((*init)->next);
  }
  if (branch != NULL)
    pNode = branch;
  else {
    pNode = (NODE*)malloc(sizeof(NODE));
    pNode->freq = freq;
    pNode->symbol = symbol;
    pNode->left = NULL;
    pNode->right = NULL;
    pNode->is_symbol = true;
  }
  pNode->next = *init;
  *init = pNode;
}

void make_list(NODE** init, unsigned long long* freq_arr) {
  for (unsigned short i = 0; i < SYMBOLS_COUNT; i++) {
    if (freq_arr[i]) {
      add_to_list(init, freq_arr[i], i, NULL);
    }
  }
}

NODE* make_node_from_two(NODE* left, NODE* right) {
  NODE* res = (NODE*)malloc(sizeof(NODE));
  res->freq = left->freq + right->freq;
  res->is_symbol = false;
  res->left = left;
  res->right = right;
  res->next = NULL;
  return res;
}

void make_tree(NODE** init) {
  while ((*init)->next) {
    NODE* new = make_node_from_two((*init), (*init)->next);
    if (((*init)->next)->next) {
      (*init) = ((*init)->next)->next;
      add_to_list(init, new->freq, '0', new);
    }
    else {
      (*init) = new;
    }
  }
}

void archive(char input_filename[], char output_filename[], unsigned long long length, NODE* init) {
  printf("start archive\n");
  unsigned char codes_len[SYMBOLS_COUNT] = {0};
  char* codes_array[SYMBOLS_COUNT] = {0}; // fixme add free
  for (long long i = 0; i < SYMBOLS_COUNT; i++) {
    codes_array[i] = (char*)calloc(SYMBOLS_COUNT, sizeof(*codes_array[i]));
  }
  for (long long i = 0; i < SYMBOLS_COUNT; i++) {
    find_and_copy_code(init, codes_array, i, &codes_len[i]);
  }
  FILE* final = fopen(output_filename, "wb");
  TRIPLE* freq_array = (TRIPLE*)malloc(SYMBOLS_COUNT * sizeof(TRIPLE));
  for (long long i = 0; i < SYMBOLS_COUNT; i++) {
    freq_array[i].freq = 0;
  }
  symmetric(init, final, freq_array);//get symbol, freq and code to array of TRIPLE
  for (int i = 0; i < SYMBOLS_COUNT; i++) { //bubble sort
    for (int j = i + 1; j < SYMBOLS_COUNT; j++) {
      if (freq_array[j].freq > freq_array[i].freq) {
        TRIPLE temp = freq_array[i];
        freq_array[i] = freq_array[j];
        freq_array[j] = temp;
      }
    }
  }

  for (int i = 0; i < 256; i++) { //print symbols and codes
    if (freq_array[i].freq == 0)
      break;
    fprintf(final, "%c:%s", freq_array[i].symbol, freq_array[i].code);
  }
  fprintf(final, "\n\n%llx\n", length);
  long long filename_pointer = (long long)strlen(input_filename) - 1;
  for (; filename_pointer > 0; filename_pointer--) {
    if (input_filename[filename_pointer] == '/') {
      filename_pointer++; //to find the beginning of filename
      break;
    }
  }
  fprintf(final, "%s\n", input_filename + filename_pointer);

  FILE* input = fopen(input_filename, "rb");
  unsigned char* fileContent = (unsigned char*)malloc(ARCHIVE_BUFF_SIZE / 256 * sizeof(unsigned char)); //~abc
  unsigned char* buffer = (unsigned char*)malloc(ARCHIVE_BUFF_SIZE * sizeof(unsigned char)); //~010101010
  unsigned char* archive = (unsigned char*)malloc(ARCHIVE_BUFF_SIZE * sizeof(unsigned char)); //~#$%^&^
  memset(fileContent, 0, sizeof(&fileContent));
  memset(buffer, 0, sizeof(&buffer));
  memset(archive, 0, sizeof(&archive));
  bool archiveStatus = true;
  unsigned long long offset = 0;
  unsigned long fileLenPercent = length/100, currentPercent = 0, cachePercent = 0, allReadBytes = 0;
  unsigned char tmpCache;
  clock_t loopStart, loopEnd;
  loopStart = clock();
  while (archiveStatus) {
    BIT_TO_CHAR symbol;
    unsigned long read_bytes = fread(fileContent, sizeof(unsigned char), ARCHIVE_BUFF_SIZE / 256, input);
    allReadBytes+=read_bytes;
    cachePercent = allReadBytes / fileLenPercent;
#ifdef ENABLE_PROGRESS
    if (cachePercent > currentPercent) {
      currentPercent = cachePercent;
      loopEnd = clock();
      printProgress(length, allReadBytes, loopEnd, loopStart);
      loopStart = clock();
    }
#endif
    if (read_bytes == 0) {
      archiveStatus = false;
      break;
    }
    for (int i = 0; i < read_bytes && offset + CODE_SIZE < ARCHIVE_BUFF_SIZE; ++i) {
      memcpy(&buffer[offset], codes_array[fileContent[i]], codes_len[fileContent[i]]);
      offset += codes_len[fileContent[i]];
    }
    long long archiveLen = 0;
    for (long long i = 0; i < offset || (offset < 8 && archiveLen == 0);) { //use offset
      symbol.mbit.b1 = buffer[i++];
      symbol.mbit.b2 = buffer[i++];
      symbol.mbit.b3 = buffer[i++];
      symbol.mbit.b4 = buffer[i++];
      symbol.mbit.b5 = buffer[i++];
      symbol.mbit.b6 = buffer[i++];
      symbol.mbit.b7 = buffer[i++];
      symbol.mbit.b8 = buffer[i++];
      archive[archiveLen++] = symbol.character;
    }
    tmpCache = offset % 8;
    memcpy(buffer, buffer+(offset-tmpCache), tmpCache); //carefully with a buffer type
    offset = tmpCache;
    fwrite(archive, sizeof(unsigned char), archiveLen, final);
    if (length == read_bytes) archiveStatus = false; // end of cycle
  }
  for (int i = 0; i < SYMBOLS_COUNT; ++i) free(codes_array[i]);
  free(freq_array);
  free(fileContent);
  free(buffer);
  free(archive);
  fclose(final);
  printf("\n");
}
