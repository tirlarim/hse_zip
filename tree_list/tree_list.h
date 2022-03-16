#ifndef TREE_LIST_TREE_LIST_H
#define TREE_LIST_TREE_LIST_H

#include <stdlib.h>
#include <stdio.h>

#define CODE_SIZE 20 //19 is max + \0
#define SYMBOLS_COUNT 256

#include <stdbool.h>

typedef struct node {
  unsigned char symbol;
  unsigned int freq;
  bool isSymbol;
  char code[CODE_SIZE];
  struct node *right, *left, *next;
}NODE;

void initTree();

#endif //TREE_LIST_TREE_LIST_H
