#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "tree_list.h"
#include "union.h"

int* init_array_with_zeroes(int count);
void get_chars_frequency(char filename[], int* freq_arr, long* length);
void add_to_list (NODE** pphead, unsigned int freq, unsigned char symbol, NODE* branch);
void make_list(NODE** init, int* freq_arr);
void make_tree(NODE** init);
void PrintTreeOnSide(const NODE* root, int level);
void printTreeCodes(const NODE* root);
void create_codes(NODE** init, int level);
void symmetric(NODE* root, FILE* file);
void find_and_print_code(NODE** init, FILE* file, unsigned char symbol);
void change_symbols_to_bits(char input_filename[], char output_filename[], long length, NODE** init); //print only
void archive(char output_filename[], long length, NODE** init);
void decode(char* filenameOutput);
void saveBuffToArr(int buffCode[255], int arr[255*2], const unsigned char* text, unsigned long textSize, int startIndex);
bool findAnswer(const int bitsArr[255], int symbolCodeArr[], int* offset);
void fillArrMinusOne(int* arr);

char code[CODE_SIZE];

void initTree(NODE* init, char* filenameInput, char* filenameOutput) {
  int* freq = init_array_with_zeroes(SYMBOLS_COUNT);
  long length = 0;
  get_chars_frequency(filenameInput,freq, &length);
  make_list(&init, freq);
  make_tree(&init);
//  PrintTreeOnSide(init, 0); // print tree
  create_codes(&init, 0);
  change_symbols_to_bits(filenameInput, filenameOutput, length, &init);
  archive(filenameOutput, length, &init);
  decode(filenameOutput);
}

//debug
void PrintTreeOnSide(const NODE* root, int level) {
  if (root) {
    PrintTreeOnSide(root->right, level + 1);
    for (int i = 0; i < level; i++) {
      printf("\t");
    }
    printf("%3d", root->freq);
    if (root->isSymbol)
      printf(":%c\n", root->symbol);
    else {
      printf("\n");
    }
    PrintTreeOnSide(root->left, level + 1);
  }
}

void symmetric(NODE* root, FILE* file) {
  if (root) {
    symmetric(root->left, file);
    if (root->isSymbol) {
      if (root->symbol != '\n' && root->symbol != '\r')
        fprintf(file, "%c:%s ", root->symbol, root->code);
      else if (root->symbol == '\n')
        fprintf(file, "%s:%s ", "\\n", root->code);
      else if (root->symbol == '\r')
        fprintf(file, "%s:%s ", "\\r", root->code);
    }
    symmetric(root->right, file);
  }
}

void create_codes(NODE** init, int level) {
  if (*init) {
    if ((*init)->isSymbol != 0) {
      code[level] = '\0';
      strcpy((*init)->code, code);
    }
    code[level] = '0';
    create_codes(&((*init)->left), level + 1);
    code[level] = '1';
    create_codes(&((*init)->right), level + 1);
  }
}

void find_and_print_code(NODE** init, FILE* file, unsigned char symbol) {
  if (*init) {
    if ((*init)->isSymbol && (unsigned char)(*init)->symbol == symbol) {
      fprintf(file, "%s", (*init)->code);
      return;
    }
    find_and_print_code(&((*init)->left), file, symbol);
    find_and_print_code(&((*init)->right), file, symbol);
  }
}


void printTreeCodes(const NODE* root) {
  if (root) {
    printTreeCodes(root->right);
    if (root->isSymbol) {
      printf("Symbol: '%c' \tfrequency: %d \tcode: ", root->symbol, root->freq);
      for (int i = 0; i < CODE_SIZE; ++i) {
        printf("%d", root->code[i]);
      }
      printf("\n");
    }
    printTreeCodes(root->left);
  }
}

int* init_array_with_zeroes(int count) {
  int* arr = (int*)malloc(count*sizeof(int));
  for (int i = 0; i < count; i++) {
    arr[i] = 0;
  }
  return arr;
}

void get_chars_frequency(char filename[], int* freq_arr, long* length) {
  FILE* input = fopen(filename, "rb");
  if (!input) exit(2);
  fseek(input, 0, SEEK_END);
  *length = ftell(input);
  fseek(input, 0, SEEK_SET);
  for (int i = 0; i < *length; i++) {
    freq_arr[(unsigned char)fgetc(input)]++;
  }
  fclose(input);
}

void add_to_list(NODE** pphead, unsigned int freq, unsigned char symbol, NODE* branch) {
  while (*pphead) {
    if ((*pphead)->freq > freq) {
      break;
    }
    pphead = &((*pphead)->next);
  }
  NODE* pnew = (NODE*)malloc(sizeof(NODE));
  pnew->freq = freq;
  pnew->symbol = symbol;
  pnew->left = NULL;
  pnew->right = NULL;
  if (branch != NULL)
    pnew = branch;
  else {
    pnew->isSymbol = true;
  }
  pnew->next = *pphead;
  *pphead = pnew;
}

void make_list(NODE** init, int* freq_arr) {
  for (int i = 0; i < SYMBOLS_COUNT; i++) {
    if (freq_arr[i]) {
      add_to_list(&(*init), freq_arr[i], i, NULL);
    }
  }
}

NODE* make_node_from_two(const NODE* left, const NODE* right) {
  NODE* res = (NODE*)malloc(sizeof(NODE));
  res->freq = left->freq + right->freq;
  res->isSymbol = false;
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

void change_symbols_to_bits(char input_filename[], char output_filename[], long length, NODE** init) {
  FILE* input = fopen(input_filename, "r");
  FILE* output = fopen(output_filename, "w");
  for (int i = 0; i < length; i++) {
    int symbol = getc(input);
    find_and_print_code(init, output, symbol);
  }
  fclose(input);
  fclose(output);
}

void archive(char output_filename[], long length, NODE** init) {
  FILE* get_codes = fopen(output_filename, "r");
  unsigned char str[length*20];
  fgets(str, length*20, get_codes);
  fclose(get_codes);
  int count = strlen(str) / BIT8;
  int tail = strlen(str) % BIT8;
  int len = count + 1;
  BIT_TO_CHAR symbol;
  unsigned char* res = (unsigned char*)malloc(len*sizeof(unsigned char));
  for (int i = 0; i < len; i++) {
    symbol.mbit.b1 = str[i*BIT8 + 0];
    symbol.mbit.b2 = str[i*BIT8 + 1];
    symbol.mbit.b3 = str[i*BIT8 + 2];
    symbol.mbit.b4 = str[i*BIT8 + 3];
    symbol.mbit.b5 = str[i*BIT8 + 4];
    symbol.mbit.b6 = str[i*BIT8 + 5];
    symbol.mbit.b7 = str[i*BIT8 + 6];
    symbol.mbit.b8 = str[i*BIT8 + 7];
    res[i] = symbol.character;
  }
  res[len] = '\0';
  FILE* final = fopen(output_filename, "w");
  symmetric(*init, final);
  fprintf(final, "\n%d", tail);
  fprintf(final, "\n%s", res);
  fclose(final);
}

void decode(char* filenameOutput) {
  int ansMaxLen = 1000;
//  init vars
  long length;
  char header[256*20] = {0};
  char ans[1000*2] = {0}; // use malloc
  int ansIndex = 0;
  int trashBits = 0;
  int codes[256][256] = {0};
  for (int i = 0; i < 256; ++i) {
    for (int j = 0; j < 256; ++j) {
      codes[i][j] = -1;
    }
  }
//  get file size
  FILE* output = fopen(filenameOutput, "rb");
  if (!output) exit(2);
  fseek(output, 0, SEEK_END);
  length = ftell(output);
  fseek(output, 0, SEEK_SET);
  fclose(output);
//  read header
  FILE* final = fopen(filenameOutput, "rb");
  fgets(header, 256*256, final);
  printf("%s\n", header);
//  create a table
  for (int i = 0; header[i] != '\n';++i) {
    unsigned char byte;
    if ((header[i] == 48 || header[i] == 49) && header[i-1] == ':') {
      if (i - 3 >= 0 && (header[i-2] == 'n' || header[i-2] == 'r') && header[i-3] == '\\') {
        byte = header[i-2] == 'n' ? '\n' : '\r';
      } else {
        byte = header[i-2];
      }
      for (int j = 0; header[i+j] != ' '; ++j) {
        codes[byte][j] = header[i+j] - '0';
      }
    }
    length--;
  }
  length-=3;
  for (int i = 0; i < 256; ++i) {
    if (codes[i][0] != -1) {
      if (!(i == 9 || i == 10 || i == 13)) {
        printf("0x%x(%c) -> ", (unsigned char)i, i);
      } else {
        switch (i) {
          case 9:
            printf("0x%x(\\t) -> ", (unsigned char)i);
            break;
          case 10:
            printf("0x%x(\\n) -> ", (unsigned char)i);
            break;
          case 13:
            printf("0x%x(\\r) -> ", (unsigned char)i);
            break;
        }
      }
    }
    for (int j = 0; codes[i][j] != -1; ++j) {
      printf("%d", codes[i][j]);
    }
    if (codes[i][0] != -1) {
      printf("\n");
    }
  }
  fscanf(final, "%d\n",  &trashBits); //  get bits count
  printf("trashBits -> %d\n", trashBits);
//  read file
  printf("len -> %ld\n", length);
  unsigned char text[length];
  memset(text, 0, sizeof(text));
  for (long i = 0; i < length; ++i) {
    text[i] = (unsigned char)fgetc(final);
//    printf("%c", text[i]);
  }
// decode
  bool decodeFlag = true;
  printf("start decode\n");
//  create array with All codes (bad solution)
  int allCodesArr[255*2];
  for (int i = 0; i < length; ++i) {
    printf("%c", text[i]);
  }
  printf("\n");
//    read it
  int buffCode[255] = {0};
  int offset = 0;
  int startIndex = 0;
  int a = 0;
  while (a < ansMaxLen) { ////////////// start
    decodeFlag = false;
    saveBuffToArr(buffCode, allCodesArr, text, sizeof(text), startIndex);
    printf("new bits: ");
    for (int i = 0; i < 255; ++i) {
      if (buffCode[i] != -1) {decodeFlag = true;}
      printf("%d", buffCode[i]);
    }
    printf("\n");
//    fill buffer
    for (int i = 0; i < 256; ++i) {
      if (codes[i][0] != -1) {
        if (findAnswer(buffCode, codes[i], &offset)) {
          ans[ansIndex++] = (char)i;
          printf("startIndex: %d\tint: %d\tsymbol: %c\n", startIndex/8, i, ans[ansIndex-1]);
          startIndex += offset;
          break;
        }
        offset = 0;
      }
    }
    a++;
//    update buffCode to 255 bits
  }
  printf("-------------text-------------\n");
  for (int i = 0; i < ansMaxLen; ++i) {
    printf("%c", ans[i]);
  }
  printf("\n");
  printf("-------------text-------------\n");
}

void saveBuffToArr(int buffCode[255], int arr[255*2], const unsigned char* text, unsigned long textSize, int startIndex) {
  int offset = startIndex/8;
  int trashBits = startIndex%8;
  BIT_TO_CHAR symbol;
  fillArrMinusOne(arr);
  for (int i = offset, j = 0; i < textSize && i < offset+32+8; ++i, ++j) {
    symbol.character = text[i];
    arr[8*j+0] = (int)symbol.mbit.b1;
    arr[8*j+1] = (int)symbol.mbit.b2;
    arr[8*j+2] = (int)symbol.mbit.b3;
    arr[8*j+3] = (int)symbol.mbit.b4;
    arr[8*j+4] = (int)symbol.mbit.b5;
    arr[8*j+5] = (int)symbol.mbit.b6;
    arr[8*j+6] = (int)symbol.mbit.b7;
    arr[8*j+7] = (int)symbol.mbit.b8;
  }
  for (int i = 0; i < 32*8-1; ++i) {
    buffCode[i] = arr[i+trashBits];
  }
}

void fillArrMinusOne(int arr[255*2]) {
  int arrLen = 255*2;
  for (int i = 0; i < arrLen; ++i) {
    arr[i] = -1;
  }
}

bool findAnswer(const int bitsArr[255], int symbolCodeArr[256], int* offset) {
  *offset = 0;
  for (int i = 0; i < 256 && symbolCodeArr[i] != -1; ++i) {
    printf("%d", symbolCodeArr[i]);
  }
  printf("\t");
  for (int i = 0; i < 256 && symbolCodeArr[i] != -1 ; ++i, ++(*offset)) {
    if (symbolCodeArr[i] != bitsArr[i]) {
      printf("NO\n");
      return false;
    }
  }
  printf("YES -> offset: %d\t", *offset);
  return true;
}
