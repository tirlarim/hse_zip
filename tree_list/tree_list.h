#ifndef TREE_LIST_TREE_LIST_H
#define TREE_LIST_TREE_LIST_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdbool.h>

#define CODE_SIZE 256
#define BYTES_COUNT 256
#define SYMBOLS_COUNT 256
#define BUFFER_SIZE 1024*1024
#define DEBUG_FLAG false
#define FORCE_DEBUG false

#define filename_buffer "../testDataOutput/buffer.txt"

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

typedef struct node {
  unsigned char symbol;
  unsigned int freq;
  bool is_symbol;
  char code[CODE_SIZE];
  struct node *right, *left, *next;
}NODE;

typedef struct codesAsTree {
  char symbol;
  bool is_symbol; //always true on last element
  struct codesAsTree *right;
  struct codesAsTree *left;
}CODES_AS_TREE;

typedef struct triple {
  unsigned char symbol;
  unsigned int freq;
  char code[CODE_SIZE];
}TRIPLE;

void init_tree(NODE* init, char* fileNameInput, char* fileNameOutput);

#endif //TREE_LIST_TREE_LIST_H
