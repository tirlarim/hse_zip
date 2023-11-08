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

void get_chars_frequency(char filename[], unsigned long long* freq_arr, unsigned long long* length);
void make_list(NODE** init, unsigned long long* freq_arr);
void make_tree(NODE** init);
void archive(char input_filename[], char output_filename[], unsigned long long length, NODE** init);
void create_codes(NODE** init, long long level, char* temp_code);

void encodeArchive(char* fileNameInput, char* fileNameOutput) {
  NODE* pBinTree = NULL;
  clock_t startTime, endTime;
  unsigned long long length = 0;
  unsigned long long freq[SYMBOLS_COUNT];
  char temp_code[CODE_SIZE];
  startTime = clock();
  memset(freq, 0, SYMBOLS_COUNT*sizeof(unsigned long long));
  memset(temp_code, 0, CODE_SIZE*sizeof(char));
  get_chars_frequency(fileNameInput,freq, &length);
  printLog("archive -> get_chars_frequency - DONE.\n");
  make_list(&pBinTree, freq);
  printLog("archive -> make_list - DONE.\n");
  make_tree(&pBinTree);
  printLog("archive -> make_tree - DONE.\n");
  create_codes(&pBinTree, 0, temp_code);
  printLog("archive -> create_codes - DONE.\n");
  archive(fileNameInput, fileNameOutput, length, &pBinTree); //take codes from buffer.txt and unite them
  printLog("archive - DONE.\n");
  endTime = clock();
  printf("archive time: %.2lf sec.\n", (double)(endTime - startTime) / (CLOCKS_PER_SEC));
}

void symmetric(NODE* init, FILE* file, TRIPLE* arr) {
  if (init) {
    symmetric(init->left, file, arr);
    if (init->is_symbol) {
      arr[(int) init->symbol].symbol = init->symbol; //array for print symbols in frequency order
      arr[(int) init->symbol].freq = init->freq;
      strcpy(arr[(int) init->symbol].code, init->code);
    }
    symmetric(init->right, file, arr);
  }
}

void get_chars_frequency(char filename[], unsigned long long* freq_arr, unsigned long long* length) {
  FILE* input = fopen(filename, "rb");
  if (!input) {
    perror("get_chars_frequency >> Unable to open file");
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
  unsigned long long bytes_read = 0;
  memset(buffer, 0, BUFFER_SIZE*sizeof(unsigned char));
  for (int i = 0; i < *length / BUFFER_SIZE; ++i) {
    bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, input);
    for (long long j = 0; j < bytes_read; ++j) {
      ++freq_arr[(unsigned int)buffer[j]];
    }
  }
  fclose(input);
}

void create_codes(NODE** init, long long level, char* temp_code) {
  if (*init) {
    if ((*init)->is_symbol != 0) {
      if (!level && !((*init)->next)) {
        (*init)->code[0] = '1';
        (*init)->code[1] = '\0';
        return;
      }
      temp_code[level] = '\0';
      strcpy((*init)->code, temp_code);
    }
    temp_code[level] = '0';
    create_codes(&((*init)->left), level + 1, temp_code);
    temp_code[level] = '1';
    create_codes(&((*init)->right), level + 1, temp_code);
  }
}

void find_and_copy_code(NODE** init, char** code_array, long long symbol) {
  if (*init) {
    if ((*init)->is_symbol && (*init)->symbol == symbol) {
      strcpy(code_array[symbol], (*init)->code);
      return;
    }
    find_and_copy_code(&((*init)->left), code_array, symbol);
    find_and_copy_code(&((*init)->right), code_array, symbol);
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

void add_to_list(NODE** init, unsigned long long freq, unsigned char symbol, NODE* branch) {
  while (*init) {
    if ((*init)->freq > freq) {
      break;
    }
    init = &((*init)->next);
  }
  NODE* pnew = (NODE*)malloc(sizeof(NODE));
  pnew->freq = freq;
  pnew->symbol = symbol;
  pnew->left = NULL;
  pnew->right = NULL;
  if (branch != NULL)
    pnew = branch;
  else {
    pnew->is_symbol = true;
  }
  pnew->next = *init;
  *init = pnew;
}

void make_list(NODE** init, unsigned long long* freq_arr) {
  for (unsigned short i = 0; i < SYMBOLS_COUNT; i++) {
    if (freq_arr[i]) {
      add_to_list(&(*init), freq_arr[i], i, NULL);
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
      add_to_list(&(*init), new->freq, '0', new);
    }
    else {
      (*init) = new;
    }
  }
}

void archive(char input_filename[], char output_filename[], unsigned long long length, NODE** init) {
  printf("start archive\n");
  char** codes_array = (char**)malloc(256*sizeof(char*));
  for (long long i = 0; i < 256; i++) {
    codes_array[i] = (char*)malloc(256*sizeof(char));
  }
  for (int i = 0; i < 256; ++i) {
    for (int j = 0; j < 256; ++j) {
      codes_array[i][j] = 0;
    }
  }
  for (long long i = 0; i < 256; i++) {
    find_and_copy_code(init, codes_array, i);
  }
  FILE* final = fopen(output_filename, "wb");
  TRIPLE* freq_array = (TRIPLE*)malloc(256*sizeof(TRIPLE));
  for (long long i = 0; i < 256; i++) {
    freq_array[i].freq = 0;
  }
  symmetric(*init, final, freq_array);//get symbol, freq and code to array of TRIPLE
  for (int i = 0; i < 256; i++) { //bubble sort
    for (int j = i + 1; j < 256; j++) {
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
  long long filename_pointer = (long long)strlen(input_filename)-1;
  for (; filename_pointer > 0; filename_pointer--) {
    if (input_filename[filename_pointer] == '/') {
      filename_pointer++; //to find beginning of filename
      break;
    }
  }
  fprintf(final, "%s\n", input_filename + filename_pointer);

  FILE* input = fopen(input_filename, "rb");
  unsigned char* fileContent = (unsigned char*)malloc(ARCHIVE_BUFF_SIZE/256*sizeof(unsigned char)); //~abc
  unsigned char* buffer = (unsigned char*)malloc(ARCHIVE_BUFF_SIZE*sizeof(unsigned char)); //~010101010
  unsigned char* archive = (unsigned char*)malloc(ARCHIVE_BUFF_SIZE*sizeof(unsigned char)); //~#$%^&^
  memset(fileContent, 0, sizeof(&fileContent));
  memset(buffer, 0, sizeof(&buffer));
  memset(archive, 0, sizeof(&archive));
  bool archiveStatus = true;
  int offset = 0;
  long long allReadBytes = 0;
  long long progressBuffer = 0;
  clock_t loopStart, loopEnd;
  loopStart = clock();
  while (archiveStatus) {
    BIT_TO_CHAR symbol;
    long long buffMemoryCurrent = 0;
    buffMemoryCurrent+=offset;
    unsigned long read_bytes = fread(fileContent, sizeof(unsigned char), ARCHIVE_BUFF_SIZE/256, input);
    allReadBytes+=(long long)read_bytes;
    if (progressBuffer != (long long)((double)allReadBytes/(double)length*100)) {
      loopEnd = clock();
      printProgress((double)allReadBytes/(double)length, (long long)(((double)(loopEnd - loopStart) / (CLOCKS_PER_SEC))*(((double)1-(double)allReadBytes/(double)length)*100)));
      progressBuffer = (long long)((double)allReadBytes/(double)length*100);
      loopStart = clock();
    }
    if (read_bytes == 0) {archiveStatus = false;}
    for (int i = 0; i < read_bytes && buffMemoryCurrent+256 < ARCHIVE_BUFF_SIZE; ++i) {
      memcpy(&buffer[buffMemoryCurrent], codes_array[(int)fileContent[i]], strlen((char*)codes_array[(int)fileContent[i]]));
      buffMemoryCurrent+=(long long)strlen((char*)codes_array[(int)fileContent[i]]);
    }
    long long archiveLen = 0;
    for (long long i = 0; i < (buffMemoryCurrent/8)*8 || (buffMemoryCurrent<8&& archiveLen==0);) { //use offset
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
    for (long long i = buffMemoryCurrent-(buffMemoryCurrent%8), j=0; j < buffMemoryCurrent%8; ++i, ++j) {
      buffer[j] = buffer[i];
    }
    offset = (int)((buffMemoryCurrent)%8);
    fwrite(archive, sizeof(unsigned char), archiveLen, final);
    if (length==read_bytes) {
      archiveStatus = false;
      break;
    }
  }
  free(freq_array);
  free(fileContent);
  free(buffer);
  free(archive);
  fclose(final);
  printf("\n");
}
