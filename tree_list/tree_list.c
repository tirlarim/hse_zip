#include <stdbool.h>
#include "tree_list.h"

int* init_array_with_zeroes(int count);
void get_chars_frequency(char filename[], int* freq_arr, long* length);
void add_to_list (NODE** pphead, unsigned int freq, int symbol, NODE* branch);
void make_list(NODE** init, int* freq_arr);
void make_tree(NODE** init);
void PrintTreeOnSide(const NODE* root, int level);
void printTreeCodes(const NODE* root);

void initTree() {
  int* freq = init_array_with_zeroes(SYMBOLS_COUNT);
  char filename[] = "../tree_list/input.txt";
  long length;
  get_chars_frequency(filename,freq, &length);
  NODE* init = NULL;
  make_list(&init, freq);
  make_tree(&init);
  //debug
//  PrintTreeOnSide(init, 0);
  printf("Codes:\n");
  printTreeCodes(init);
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

void printTreeCodes(const NODE* root) {
  if (root) {
    printTreeCodes(root->right);
    if (root->isSymbol) {
      printf("Symbol: '%c'\t", root->symbol);
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
