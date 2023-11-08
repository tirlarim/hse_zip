#ifndef TREE_LIST_DECODE_H
#define TREE_LIST_DECODE_H

#define DECODE_BUFF_SIZE (8*10000)
#define BYTES_COUNT 256

typedef struct codesAsTree {
  char symbol;
  bool is_symbol; //always true on last element
  struct codesAsTree *right;
  struct codesAsTree *left;
}CODES_AS_TREE;

void decode(char* archiveFilename);

#endif //TREE_LIST_DECODE_H