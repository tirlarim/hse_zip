#ifndef TREE_LIST_DECODE_H
#define TREE_LIST_DECODE_H

#include "./tree_list.h"

#define DECODE_BUFF_SIZE (8*100000)
#define BYTES_COUNT 256
#define ANSWER_BUFFER_SIZE 100000

typedef struct codesAsTree {
  char symbol;
  bool is_symbol; //always true on last element
  struct codesAsTree *right, *left;
}CODES_AS_TREE;

void decodeArchive(char* fileNameOutput);

#endif //TREE_LIST_DECODE_H