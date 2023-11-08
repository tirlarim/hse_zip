#ifndef TREE_LIST_ENCODE_H
#define TREE_LIST_ENCODE_H

#define ARCHIVE_BUFF_SIZE 800000
#define BUFFER_SIZE (1024*1024)
#define SYMBOLS_COUNT 256 // possible different symbols [0:255]


typedef struct node {
  unsigned char symbol; // symbol: a
  unsigned int freq;
  bool is_symbol;
  char code[CODE_SIZE]; // binary symbol code example: 1010101
  struct node *right, *left, *next;
}NODE;

typedef struct triple {
  unsigned char symbol;
  unsigned int freq;
  char code[CODE_SIZE];
}TRIPLE;

void encodeArchive(char* fileNameInput, char* fileNameOutput);

#endif //TREE_LIST_ENCODE_H