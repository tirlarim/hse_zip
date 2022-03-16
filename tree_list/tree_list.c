#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "tree_list.h"
#include "union.h"

int* init_array_with_zeroes(int count);
void get_chars_frequency(char filename[], int* freq_arr, long* length);
void add_to_list (NODE** pphead, unsigned int freq, int symbol, NODE* branch);
void make_list(NODE** init, int* freq_arr);
void make_tree(NODE** init);
void PrintTreeOnSide(const NODE* root, int level);
void printTreeCodes(const NODE* root);
void create_codes(NODE** init, int level);
void Simmetric(const NODE* root);
void find_and_print_code(NODE** init, FILE* file, int symbol);

char code[CODE_SIZE];

void initTree(NODE* init, char* filenameInput, char* filenameOutput, char* filenameOutputUnzip) {
  int* freq = init_array_with_zeroes(SYMBOLS_COUNT);
  long length = 0;
  get_chars_frequency(filenameInput,freq, &length);
  make_list(&init, freq);
  make_tree(&init);
  //debug
  PrintTreeOnSide(init, 0);
  //endofdebug
  create_codes(&init, 0);
  //debug
  printf("\n");
  Simmetric(init);
  //endofdebug

  //замена символов на биты
  FILE* input = fopen(filenameInput, "r");
  FILE* output = fopen(filenameOutput, "w");
  for (int i = 0; i < length; i++) {
    int symbol = getc(input);
    find_and_print_code(&init, output, symbol);
  }
  fclose(input);
  fclose(output);

  //union - заготовка
  FILE* fir_output = fopen(filenameOutput, "r");
  unsigned char str[length*10];
  fgets(str, length*10, fir_output); //length - long!
  fclose(fir_output);
  int count = strlen(str) / BIT8;
  int tail = strlen(str) % BIT8; // not used
  int len = count + 1;
  BIT_TO_CHAR symbol;
  unsigned char* res = (unsigned char*)malloc(len*sizeof(unsigned char));
  for (int i = 0; i < len; i++) {
    symbol.mbit.b1 = str[i * BIT8 + 0];
    symbol.mbit.b2 = str[i * BIT8 + 1];
    symbol.mbit.b3 = str[i * BIT8 + 2];
    symbol.mbit.b4 = str[i * BIT8 + 3];
    symbol.mbit.b5 = str[i * BIT8 + 4];
    symbol.mbit.b6 = str[i * BIT8 + 5];
    symbol.mbit.b7 = str[i * BIT8 + 6];
    symbol.mbit.b8 = str[i * BIT8 + 7];
    res[i] = symbol.symbol;
  }
  FILE* sec_output = fopen(filenameOutputUnzip, "w");
  fprintf(sec_output, "%s", res);
  fclose(sec_output);
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

void Simmetric(const NODE* root) {
  if (root) {
    Simmetric(root->left);
    if (root->isSymbol)
      printf("%c: %s\n", root->symbol, root->code);
    Simmetric(root->right);
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

void find_and_print_code(NODE** init, FILE* file, int symbol) {
  if (*init) {
    if ((*init)->isSymbol && (int)(*init)->symbol == symbol) {
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

void add_to_list(NODE** pphead, unsigned int freq, int symbol, NODE* branch) {
  while (*pphead) {
    if ((*pphead)->freq > freq) {
      break;
    }
    pphead = &((*pphead)->next);
  }
  NODE* pnew = (NODE*)malloc(sizeof(NODE));
  pnew->freq = freq;
  pnew->symbol = (char) symbol;
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
