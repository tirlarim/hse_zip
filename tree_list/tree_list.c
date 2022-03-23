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
  fprintf(final, "%d", tail);
  fprintf(final, "\n%s", res);
  fclose(final);
}