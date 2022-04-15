#include "tree_list.h"
#include "union.h"
#include <time.h>

#define filename_buffer "../testDataOutput/buffer.txt"

int* init_array_with_zeroes(int count);
void get_chars_frequency(char filename[], int* freq_arr, long* length);
void add_to_list (NODE** init, unsigned int freq, unsigned char symbol, NODE* branch);
void make_list(NODE** init, int* freq_arr);
void make_tree(NODE** init);
void print_tree_on_side(const NODE* init, int level);
void create_codes(NODE** init, int level);
void symmetric(NODE* init, FILE* file);
void find_and_copy_code(NODE** init, char** code_array, int symbol);
void change_symbols_to_codes(char input_filename[], char output_filename[], long length, NODE** init);
void archive(char input_filename[], char output_filename[], long length, NODE** init);
void decode(char* fileNameOutput);
void printTreeCodes(const NODE* init);
void prepareBytesBuffer(int buffCode[256+8], FILE* fp, int lastOffset, unsigned long* fileLen);
bool findAnswer(const int bitsArr[256], int symbolCodeArr[], int* offset, int* codeLen);
void fillArrMinusOne(int* arr);

char code[CODE_SIZE];

void init_tree(NODE* init, char* fileNameInput, char* fileNameOutput) {
  int* freq = init_array_with_zeroes(SYMBOLS_COUNT);
  long length = 0;
  get_chars_frequency(fileNameInput,freq, &length);
  make_list(&init, freq);
  make_tree(&init);
//  print_tree_on_side(init, 0); // print tree
  create_codes(&init, 0);
  change_symbols_to_codes(fileNameInput, filename_buffer, length, &init);
  archive(fileNameInput, fileNameOutput, length, &init);
  decode(fileNameOutput);
}

//debug
void print_tree_on_side(const NODE* init, int level) {
  if (init) {
    print_tree_on_side(init->right, level + 1);
    for (int i = 0; i < level; i++) {
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

void symmetric(NODE* init, FILE* file) {
  if (init) {
    symmetric(init->left, file);
    if (init->is_symbol) {
      if (init->symbol != '\n' && init->symbol != '\r')
        fprintf(file, "%c:%s ", init->symbol, init->code);
      else if (init->symbol == '\n')
        fprintf(file, "%s:%s ", "\\n", init->code);
      else if (init->symbol == '\r')
        fprintf(file, "%s:%s ", "\\r", init->code);
    }
    symmetric(init->right, file);
  }
}

void create_codes(NODE** init, int level) {
  if (*init) {
    if ((*init)->is_symbol != 0) {
      if (!level && !((*init)->next)) {
        (*init)->code[0] = '1';
        (*init)->code[1] = '\0';
        return;
      }
      code[level] = '\0';
      strcpy((*init)->code, code);
    }
    code[level] = '0';
    create_codes(&((*init)->left), level + 1);
    code[level] = '1';
    create_codes(&((*init)->right), level + 1);
  }
}

void find_and_copy_code(NODE** init, char** code_array, int symbol) {
  if (*init) {
    if ((*init)->is_symbol && (*init)->symbol == symbol) {
      strcpy(code_array[symbol], (*init)->code);
      return;
    }
    find_and_copy_code(&((*init)->left), code_array, symbol);
    find_and_copy_code(&((*init)->right), code_array, symbol);
  }
}


void printTreeCodes(const NODE* init) {
  if (init) {
    printTreeCodes(init->right);
    if (init->is_symbol) {
      printf("Symbol: '%c' \tfrequency: %d \tcode: ", init->symbol, init->freq);
      for (int i = 0; i < CODE_SIZE; ++i) {
        printf("%d", init->code[i]);
      }
      printf("\n");
    }
    printTreeCodes(init->left);
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

void add_to_list(NODE** init, unsigned int freq, unsigned char symbol, NODE* branch) {
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

void make_list(NODE** init, int* freq_arr) {
  for (int i = 0; i < SYMBOLS_COUNT; i++) {
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

void change_symbols_to_codes(char input_filename[], char output_filename[], long length, NODE** init) {
  char** codes_array = (char**)malloc(256*sizeof(char*));
  for (int i = 0; i < 256; i++) {
    codes_array[i] = (char*)malloc(256*sizeof(char));
  }
  for (int i = 0; i < 256; i++) {
    find_and_copy_code(init, codes_array, i);
  }

  FILE* input = fopen(input_filename, "rb");
  FILE* output = fopen(output_filename, "w+");
  unsigned char buffer[BUFFER_SIZE];
  int first_time = 1;
  while (length > 0 || first_time) {
    length -= BUFFER_SIZE;
    first_time = 0;
    unsigned long long read_bytes = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, input);
    for (int i = 0; i < read_bytes; i++) {
      fprintf(output, "%s", codes_array[(int)buffer[i]]);
    }
  }
  fclose(input);
  fclose(output);
}

void archive(char input_filename[], char output_filename[], long length, NODE** init) {
  FILE* get_codes = fopen(filename_buffer, "rb");
  fseek(get_codes, 0, SEEK_END);
  int count = ftell(get_codes);
  fseek(get_codes, 0, SEEK_SET);
  int len = count / 8 + 1;
  int tail = len * 8 - count;
  if (count % 8 == 0) len--, tail = 0;

  FILE* final = fopen(output_filename, "w");
  symmetric(*init, final);
  fprintf(final, "\n%ld", length);

  int filename_pointer = (int) strlen(input_filename);
  filename_pointer--;
  for (; filename_pointer > 0; filename_pointer--) {
    if (input_filename[filename_pointer] == '/') {
      filename_pointer++;
      break;
    }
  }

  fprintf(final, "\n%s\n", input_filename + filename_pointer);
  unsigned long long bytes_read = 0;
  unsigned char buffer[BUFFER_SIZE];
  memset(buffer, '0', BUFFER_SIZE);
  int first_time = 1;
  while (count > 0 || first_time) {
    first_time = 0;
    count -= BUFFER_SIZE;
    memset(buffer, '0', bytes_read);
    bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, get_codes);
    int i = 0;
    int buffer_pointer = 0;
    BIT_TO_CHAR symbol;
    while (i < bytes_read) {
      symbol.mbit.b1 = buffer[i+0];
      symbol.mbit.b2 = buffer[i+1];
      symbol.mbit.b3 = buffer[i+2];
      symbol.mbit.b4 = buffer[i+3];
      symbol.mbit.b5 = buffer[i+4];
      symbol.mbit.b6 = buffer[i+5];
      symbol.mbit.b7 = buffer[i+6];
      symbol.mbit.b8 = buffer[i+7];
      buffer[buffer_pointer] = symbol.character;
      i+= 8;
      buffer_pointer++;
    }
    fwrite(buffer, sizeof(unsigned char), buffer_pointer, final);
  }
  fclose(get_codes);
  fclose(final);
  remove(filename_buffer);
}

void decode(char* fileNameOutput) {
  clock_t startTime, endTime;
  startTime = clock();
  char header[BYTES_COUNT*CODE_SIZE] = {0};
  char outputFileName[1000] = "../testDataOutput/";
  int ansIndex = 0;
  unsigned long fileNameLength = 0;
  char decodeFileName[1000] = {0};
  int decodeFileSizeBytes = 0;
  int codes[BYTES_COUNT][CODE_SIZE] = {0};
  for (int i = 0; i < CODE_SIZE; ++i) {
    for (int j = 0; j < CODE_SIZE; ++j) {
      codes[i][j] = -1;
    }
  }
//  get file size
  FILE* output = fopen(fileNameOutput, "rb");
  if (!output) exit(2);
  fseek(output, 0, SEEK_END);
  unsigned long length = ftell(output);
  fseek(output, 0, SEEK_SET);
  fclose(output);
//  read header
  FILE* final = fopen(fileNameOutput, "rb");
  fgets(header, BYTES_COUNT*CODE_SIZE, final);
  printf("%s\n", header);
//  create a table
  for (int i = 0; header[i] != '\n'; ++i) {
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
  length-=5;
  for (int i = 0; i < CODE_SIZE; ++i) {
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
  fscanf(final, "%d\n",  &decodeFileSizeBytes); //  get bits count
  printf("file size -> %d\n", decodeFileSizeBytes);
  fscanf(final, "%s\n",  decodeFileName); //  get bits count
  printf("file name -> %s\n", decodeFileName);
  fileNameLength = strlen(decodeFileName);
  length -= fileNameLength;
  printf("len -> %ld\n", length);
  char ans[1000+1];
//  char ans[decodeFileSizeBytes];
  printf("start decode\n");
//  int allCodesArr[256*2];
  int buffCode[256+8] = {0};
  int offset = 0;
  int startIndex = 0;
  int lastOffset = 0;
  int a = 0;
  unsigned long fileCurrentLen = length;
  BIT_TO_CHAR symbol;
  fillArrMinusOne(buffCode);
  for (int i = 0; i < 32 && i < length; ++i) {
    symbol.character = (unsigned char)getc(final);
    buffCode[8*i+0] = (int)symbol.mbit.b1;
    buffCode[8*i+1] = (int)symbol.mbit.b2;
    buffCode[8*i+2] = (int)symbol.mbit.b3;
    buffCode[8*i+3] = (int)symbol.mbit.b4;
    buffCode[8*i+4] = (int)symbol.mbit.b5;
    buffCode[8*i+5] = (int)symbol.mbit.b6;
    buffCode[8*i+6] = (int)symbol.mbit.b7;
    buffCode[8*i+7] = (int)symbol.mbit.b8;
    --fileCurrentLen;
  }
//  printf("new bits: ");
//  for (int i = 0; i < 256; ++i) {
//    printf("%d", buffCode[i]);
//  }
//  printf("|");
//  for (int i = 256; i < 256+8; ++i) {
//    printf("%d", buffCode[i]);
//  }
//  printf("\n");
  strncat(outputFileName, decodeFileName, sizeof(outputFileName) - fileNameLength - 1);
  FILE *fp = fopen(outputFileName, "wb" );
  bool writeFlag = true;
  while (writeFlag) { ////////////// start
    writeFlag = false;
    for (int i = 0; i < 256; ++i) {
      if (codes[i][0] != -1) {
        if (findAnswer(buffCode, codes[i], &offset, &lastOffset)) {
          writeFlag = true;
          ans[ansIndex++] = (char)i;
//          printf("symbol: %c\tcode: ", i);
//          for (int j = 0; codes[i][j] != -1; ++j) {
//            printf("%d", codes[i][j]);
//          }
//          printf("\n");
          startIndex += offset;
          if (a % 1000 == 0 || a == decodeFileSizeBytes-1) {
            fwrite(ans , 1, ansIndex, fp);
            ansIndex = 0;
            memset(ans, 0, sizeof(ans));
          }
          break;
        }
      }
    }
    prepareBytesBuffer(buffCode, final, lastOffset, &fileCurrentLen);
//    printf("new bits: ");
//    for (int i = 0; i < 256; ++i) {
//      printf("%d", buffCode[i]);
//    }
//    printf("|");
//    for (int i = 256; i < 256+8; ++i) {
//      printf("%d", buffCode[i]);
//    }
//    printf("\n");
    a++;
  }
  fclose(fp);
  endTime = clock();
  printf("decode time: %.2lf sec.\n", (double)(endTime - startTime) / (CLOCKS_PER_SEC));
}

void prepareBytesBuffer(int buffCode[256+8], FILE* fp, int lastOffset, unsigned long* fileLen) {
  int trashBitesCount = 0;
  bool writeFlag = true;
  BIT_TO_CHAR symbol;
  for (int i = 256; i < 256+8; ++i) {
    if (buffCode[i] != -1) {
      ++trashBitesCount;
    }
  }
  for (int i = 0; i < 256 + 8 - lastOffset; ++i) {
    buffCode[i] = buffCode[i+lastOffset];
  }
//  printf("offset: %d\t", lastOffset);
  for (int i = 256+8-lastOffset; i < 256+8; ++i) {
    buffCode[i] = -1;
  }
  for (int i = 256; i < 256+8; ++i) {
    if (buffCode[i] != -1) {
      writeFlag = false;
      break;
    }
  }
  if (writeFlag && buffCode[256-1] == -1) {
    lastOffset -= trashBitesCount;
//    printf("write: Yes\n");
    for (int i = (256 - lastOffset)/8, j = 0; j < lastOffset; ++i, j+=8) {
      if (*fileLen > 0) {
        symbol.character = (unsigned char)getc(fp);
        --(*fileLen);
        buffCode[lastOffset%8 != 0 ? 8*i+0+(8-(lastOffset%8)) : 8*i+0] = (int)symbol.mbit.b1;
        buffCode[lastOffset%8 != 0 ? 8*i+1+(8-(lastOffset%8)) : 8*i+1] = (int)symbol.mbit.b2;
        buffCode[lastOffset%8 != 0 ? 8*i+2+(8-(lastOffset%8)) : 8*i+2] = (int)symbol.mbit.b3;
        buffCode[lastOffset%8 != 0 ? 8*i+3+(8-(lastOffset%8)) : 8*i+3] = (int)symbol.mbit.b4;
        buffCode[lastOffset%8 != 0 ? 8*i+4+(8-(lastOffset%8)) : 8*i+4] = (int)symbol.mbit.b5;
        buffCode[lastOffset%8 != 0 ? 8*i+5+(8-(lastOffset%8)) : 8*i+5] = (int)symbol.mbit.b6;
        buffCode[lastOffset%8 != 0 ? 8*i+6+(8-(lastOffset%8)) : 8*i+6] = (int)symbol.mbit.b7;
        buffCode[lastOffset%8 != 0 ? 8*i+7+(8-(lastOffset%8)) : 8*i+7] = (int)symbol.mbit.b8;
      } else {
        buffCode[lastOffset%8 != 0 ? 8*i+0+(8-(lastOffset%8)) : 8*i+0] = -1;
        buffCode[lastOffset%8 != 0 ? 8*i+1+(8-(lastOffset%8)) : 8*i+1] = -1;
        buffCode[lastOffset%8 != 0 ? 8*i+2+(8-(lastOffset%8)) : 8*i+2] = -1;
        buffCode[lastOffset%8 != 0 ? 8*i+3+(8-(lastOffset%8)) : 8*i+3] = -1;
        buffCode[lastOffset%8 != 0 ? 8*i+4+(8-(lastOffset%8)) : 8*i+4] = -1;
        buffCode[lastOffset%8 != 0 ? 8*i+5+(8-(lastOffset%8)) : 8*i+5] = -1;
        buffCode[lastOffset%8 != 0 ? 8*i+6+(8-(lastOffset%8)) : 8*i+6] = -1;
        buffCode[lastOffset%8 != 0 ? 8*i+7+(8-(lastOffset%8)) : 8*i+7] = -1;
      }

    }
  } else {
//    printf("write: No\n");
  }
}

void fillArrMinusOne(int arr[256*2]) {
  int arrLen = 256*2;
  for (int i = 0; i < arrLen; ++i) {
    arr[i] = -1;
  }
}

bool findAnswer(const int bitsArr[256], int symbolCodeArr[256], int* offset, int* codeLen) {
  *offset = 0;
  for (int i = 0; symbolCodeArr[i] != -1 && i < 256 ; ++i, ++(*offset)) {
    if (symbolCodeArr[i] != bitsArr[i]) {
      return false;
    }
    *codeLen = i+1;
  }
//  printf("codeLen: %d\t", *codeLen);
  return true;
}
