#include "tree_list.h"
#include "union.h"

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
void printCodes(CHARS_TO_BITS* buffer);
void saveBuffToArr(int buffCode[255], int arr[255*2], const unsigned char* text, unsigned long textSize, int startIndex);
bool findAnswer(const int bitsArr[255], int symbolCodeArr[], int* offset);
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
  fprintf(final, "\n%d\n", tail);
//  fprintf(final, "\n%s\n", input_filename);

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
  FILE* output = fopen(fileNameOutput, "rb");
  if (!output) exit(2);
  fseek(output, 0, SEEK_END);
  length = ftell(output);
  fseek(output, 0, SEEK_SET);
  fclose(output);
//  read header
  FILE* final = fopen(fileNameOutput, "rb");
  fgets(header, 256*20, final);
  printf("%s\n", header);
//  create a table
  for (int i = 0; header[i] != '\n';++i) {
    unsigned char byte;
    int codeArr[20];
    for (int j = 0; j < 20; ++j) {codeArr[j] = -1;}
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

void printCodes(CHARS_TO_BITS* buffer) {
  printf("%s\n", buffer->characters);
  printf("%d", (int)buffer->BITS_255.b1);
  printf("%d", (int)buffer->BITS_255.b2);
  printf("%d", (int)buffer->BITS_255.b3);
  printf("%d", (int)buffer->BITS_255.b4);
  printf("%d", (int)buffer->BITS_255.b5);
  printf("%d", (int)buffer->BITS_255.b6);
  printf("%d", (int)buffer->BITS_255.b7);
  printf("%d", (int)buffer->BITS_255.b8);
  printf("%d", (int)buffer->BITS_255.b9);
  printf("%d", (int)buffer->BITS_255.b10);
  printf("%d", (int)buffer->BITS_255.b11);
  printf("%d", (int)buffer->BITS_255.b12);
  printf("%d", (int)buffer->BITS_255.b13);
  printf("%d", (int)buffer->BITS_255.b14);
  printf("%d", (int)buffer->BITS_255.b15);
  printf("%d", (int)buffer->BITS_255.b16);
  printf("%d", (int)buffer->BITS_255.b17);
  printf("%d", (int)buffer->BITS_255.b18);
  printf("%d", (int)buffer->BITS_255.b19);
  printf("%d", (int)buffer->BITS_255.b20);
  printf("%d", (int)buffer->BITS_255.b21);
  printf("%d", (int)buffer->BITS_255.b22);
  printf("%d", (int)buffer->BITS_255.b23);
  printf("%d", (int)buffer->BITS_255.b24);
  printf("%d", (int)buffer->BITS_255.b25);
  printf("%d", (int)buffer->BITS_255.b26);
  printf("%d", (int)buffer->BITS_255.b27);
  printf("%d", (int)buffer->BITS_255.b28);
  printf("%d", (int)buffer->BITS_255.b29);
  printf("%d", (int)buffer->BITS_255.b30);
  printf("%d", (int)buffer->BITS_255.b31);
  printf("%d", (int)buffer->BITS_255.b32);
  printf("%d", (int)buffer->BITS_255.b33);
  printf("%d", (int)buffer->BITS_255.b34);
  printf("%d", (int)buffer->BITS_255.b35);
  printf("%d", (int)buffer->BITS_255.b36);
  printf("%d", (int)buffer->BITS_255.b37);
  printf("%d", (int)buffer->BITS_255.b38);
  printf("%d", (int)buffer->BITS_255.b39);
  printf("%d", (int)buffer->BITS_255.b40);
  printf("%d", (int)buffer->BITS_255.b41);
  printf("%d", (int)buffer->BITS_255.b42);
  printf("%d", (int)buffer->BITS_255.b43);
  printf("%d", (int)buffer->BITS_255.b44);
  printf("%d", (int)buffer->BITS_255.b45);
  printf("%d", (int)buffer->BITS_255.b46);
  printf("%d", (int)buffer->BITS_255.b47);
  printf("%d", (int)buffer->BITS_255.b48);
  printf("%d", (int)buffer->BITS_255.b49);
  printf("%d", (int)buffer->BITS_255.b50);
  printf("%d", (int)buffer->BITS_255.b51);
  printf("%d", (int)buffer->BITS_255.b52);
  printf("%d", (int)buffer->BITS_255.b53);
  printf("%d", (int)buffer->BITS_255.b54);
  printf("%d", (int)buffer->BITS_255.b55);
  printf("%d", (int)buffer->BITS_255.b56);
  printf("%d", (int)buffer->BITS_255.b57);
  printf("%d", (int)buffer->BITS_255.b58);
  printf("%d", (int)buffer->BITS_255.b59);
  printf("%d", (int)buffer->BITS_255.b60);
  printf("%d", (int)buffer->BITS_255.b61);
  printf("%d", (int)buffer->BITS_255.b62);
  printf("%d", (int)buffer->BITS_255.b63);
  printf("%d", (int)buffer->BITS_255.b64);
  printf("%d", (int)buffer->BITS_255.b65);
  printf("%d", (int)buffer->BITS_255.b66);
  printf("%d", (int)buffer->BITS_255.b67);
  printf("%d", (int)buffer->BITS_255.b68);
  printf("%d", (int)buffer->BITS_255.b69);
  printf("%d", (int)buffer->BITS_255.b70);
  printf("%d", (int)buffer->BITS_255.b71);
  printf("%d", (int)buffer->BITS_255.b72);
  printf("%d", (int)buffer->BITS_255.b73);
  printf("%d", (int)buffer->BITS_255.b74);
  printf("%d", (int)buffer->BITS_255.b75);
  printf("%d", (int)buffer->BITS_255.b76);
  printf("%d", (int)buffer->BITS_255.b77);
  printf("%d", (int)buffer->BITS_255.b78);
  printf("%d", (int)buffer->BITS_255.b79);
  printf("%d", (int)buffer->BITS_255.b80);
  printf("%d", (int)buffer->BITS_255.b81);
  printf("%d", (int)buffer->BITS_255.b82);
  printf("%d", (int)buffer->BITS_255.b83);
  printf("%d", (int)buffer->BITS_255.b84);
  printf("%d", (int)buffer->BITS_255.b85);
  printf("%d", (int)buffer->BITS_255.b86);
  printf("%d", (int)buffer->BITS_255.b87);
  printf("%d", (int)buffer->BITS_255.b88);
  printf("%d", (int)buffer->BITS_255.b89);
  printf("%d", (int)buffer->BITS_255.b90);
  printf("%d", (int)buffer->BITS_255.b91);
  printf("%d", (int)buffer->BITS_255.b92);
  printf("%d", (int)buffer->BITS_255.b93);
  printf("%d", (int)buffer->BITS_255.b94);
  printf("%d", (int)buffer->BITS_255.b95);
  printf("%d", (int)buffer->BITS_255.b96);
  printf("%d", (int)buffer->BITS_255.b97);
  printf("%d", (int)buffer->BITS_255.b98);
  printf("%d", (int)buffer->BITS_255.b99);
  printf("%d", (int)buffer->BITS_255.b100);
  printf("%d", (int)buffer->BITS_255.b101);
  printf("%d", (int)buffer->BITS_255.b102);
  printf("%d", (int)buffer->BITS_255.b103);
  printf("%d", (int)buffer->BITS_255.b104);
  printf("%d", (int)buffer->BITS_255.b105);
  printf("%d", (int)buffer->BITS_255.b106);
  printf("%d", (int)buffer->BITS_255.b107);
  printf("%d", (int)buffer->BITS_255.b108);
  printf("%d", (int)buffer->BITS_255.b109);
  printf("%d", (int)buffer->BITS_255.b110);
  printf("%d", (int)buffer->BITS_255.b111);
  printf("%d", (int)buffer->BITS_255.b112);
  printf("%d", (int)buffer->BITS_255.b113);
  printf("%d", (int)buffer->BITS_255.b114);
  printf("%d", (int)buffer->BITS_255.b115);
  printf("%d", (int)buffer->BITS_255.b116);
  printf("%d", (int)buffer->BITS_255.b117);
  printf("%d", (int)buffer->BITS_255.b118);
  printf("%d", (int)buffer->BITS_255.b119);
  printf("%d", (int)buffer->BITS_255.b120);
  printf("%d", (int)buffer->BITS_255.b121);
  printf("%d", (int)buffer->BITS_255.b122);
  printf("%d", (int)buffer->BITS_255.b123);
  printf("%d", (int)buffer->BITS_255.b124);
  printf("%d", (int)buffer->BITS_255.b125);
  printf("%d", (int)buffer->BITS_255.b126);
  printf("%d", (int)buffer->BITS_255.b127);
  printf("%d", (int)buffer->BITS_255.b128);
  printf("%d", (int)buffer->BITS_255.b129);
  printf("%d", (int)buffer->BITS_255.b130);
  printf("%d", (int)buffer->BITS_255.b131);
  printf("%d", (int)buffer->BITS_255.b132);
  printf("%d", (int)buffer->BITS_255.b133);
  printf("%d", (int)buffer->BITS_255.b134);
  printf("%d", (int)buffer->BITS_255.b135);
  printf("%d", (int)buffer->BITS_255.b136);
  printf("%d", (int)buffer->BITS_255.b137);
  printf("%d", (int)buffer->BITS_255.b138);
  printf("%d", (int)buffer->BITS_255.b139);
  printf("%d", (int)buffer->BITS_255.b140);
  printf("%d", (int)buffer->BITS_255.b141);
  printf("%d", (int)buffer->BITS_255.b142);
  printf("%d", (int)buffer->BITS_255.b143);
  printf("%d", (int)buffer->BITS_255.b144);
  printf("%d", (int)buffer->BITS_255.b145);
  printf("%d", (int)buffer->BITS_255.b146);
  printf("%d", (int)buffer->BITS_255.b147);
  printf("%d", (int)buffer->BITS_255.b148);
  printf("%d", (int)buffer->BITS_255.b149);
  printf("%d", (int)buffer->BITS_255.b150);
  printf("%d", (int)buffer->BITS_255.b151);
  printf("%d", (int)buffer->BITS_255.b152);
  printf("%d", (int)buffer->BITS_255.b153);
  printf("%d", (int)buffer->BITS_255.b154);
  printf("%d", (int)buffer->BITS_255.b155);
  printf("%d", (int)buffer->BITS_255.b156);
  printf("%d", (int)buffer->BITS_255.b157);
  printf("%d", (int)buffer->BITS_255.b158);
  printf("%d", (int)buffer->BITS_255.b159);
  printf("%d", (int)buffer->BITS_255.b160);
  printf("%d", (int)buffer->BITS_255.b161);
  printf("%d", (int)buffer->BITS_255.b162);
  printf("%d", (int)buffer->BITS_255.b163);
  printf("%d", (int)buffer->BITS_255.b164);
  printf("%d", (int)buffer->BITS_255.b165);
  printf("%d", (int)buffer->BITS_255.b166);
  printf("%d", (int)buffer->BITS_255.b167);
  printf("%d", (int)buffer->BITS_255.b168);
  printf("%d", (int)buffer->BITS_255.b169);
  printf("%d", (int)buffer->BITS_255.b170);
  printf("%d", (int)buffer->BITS_255.b171);
  printf("%d", (int)buffer->BITS_255.b172);
  printf("%d", (int)buffer->BITS_255.b173);
  printf("%d", (int)buffer->BITS_255.b174);
  printf("%d", (int)buffer->BITS_255.b175);
  printf("%d", (int)buffer->BITS_255.b176);
  printf("%d", (int)buffer->BITS_255.b177);
  printf("%d", (int)buffer->BITS_255.b178);
  printf("%d", (int)buffer->BITS_255.b179);
  printf("%d", (int)buffer->BITS_255.b180);
  printf("%d", (int)buffer->BITS_255.b181);
  printf("%d", (int)buffer->BITS_255.b182);
  printf("%d", (int)buffer->BITS_255.b183);
  printf("%d", (int)buffer->BITS_255.b184);
  printf("%d", (int)buffer->BITS_255.b185);
  printf("%d", (int)buffer->BITS_255.b186);
  printf("%d", (int)buffer->BITS_255.b187);
  printf("%d", (int)buffer->BITS_255.b188);
  printf("%d", (int)buffer->BITS_255.b189);
  printf("%d", (int)buffer->BITS_255.b190);
  printf("%d", (int)buffer->BITS_255.b191);
  printf("%d", (int)buffer->BITS_255.b192);
  printf("%d", (int)buffer->BITS_255.b193);
  printf("%d", (int)buffer->BITS_255.b194);
  printf("%d", (int)buffer->BITS_255.b195);
  printf("%d", (int)buffer->BITS_255.b196);
  printf("%d", (int)buffer->BITS_255.b197);
  printf("%d", (int)buffer->BITS_255.b198);
  printf("%d", (int)buffer->BITS_255.b199);
  printf("%d", (int)buffer->BITS_255.b200);
  printf("%d", (int)buffer->BITS_255.b201);
  printf("%d", (int)buffer->BITS_255.b202);
  printf("%d", (int)buffer->BITS_255.b203);
  printf("%d", (int)buffer->BITS_255.b204);
  printf("%d", (int)buffer->BITS_255.b205);
  printf("%d", (int)buffer->BITS_255.b206);
  printf("%d", (int)buffer->BITS_255.b207);
  printf("%d", (int)buffer->BITS_255.b208);
  printf("%d", (int)buffer->BITS_255.b209);
  printf("%d", (int)buffer->BITS_255.b210);
  printf("%d", (int)buffer->BITS_255.b211);
  printf("%d", (int)buffer->BITS_255.b212);
  printf("%d", (int)buffer->BITS_255.b213);
  printf("%d", (int)buffer->BITS_255.b214);
  printf("%d", (int)buffer->BITS_255.b215);
  printf("%d", (int)buffer->BITS_255.b216);
  printf("%d", (int)buffer->BITS_255.b217);
  printf("%d", (int)buffer->BITS_255.b218);
  printf("%d", (int)buffer->BITS_255.b219);
  printf("%d", (int)buffer->BITS_255.b220);
  printf("%d", (int)buffer->BITS_255.b221);
  printf("%d", (int)buffer->BITS_255.b222);
  printf("%d", (int)buffer->BITS_255.b223);
  printf("%d", (int)buffer->BITS_255.b224);
  printf("%d", (int)buffer->BITS_255.b225);
  printf("%d", (int)buffer->BITS_255.b226);
  printf("%d", (int)buffer->BITS_255.b227);
  printf("%d", (int)buffer->BITS_255.b228);
  printf("%d", (int)buffer->BITS_255.b229);
  printf("%d", (int)buffer->BITS_255.b230);
  printf("%d", (int)buffer->BITS_255.b231);
  printf("%d", (int)buffer->BITS_255.b232);
  printf("%d", (int)buffer->BITS_255.b233);
  printf("%d", (int)buffer->BITS_255.b234);
  printf("%d", (int)buffer->BITS_255.b235);
  printf("%d", (int)buffer->BITS_255.b236);
  printf("%d", (int)buffer->BITS_255.b237);
  printf("%d", (int)buffer->BITS_255.b238);
  printf("%d", (int)buffer->BITS_255.b239);
  printf("%d", (int)buffer->BITS_255.b240);
  printf("%d", (int)buffer->BITS_255.b241);
  printf("%d", (int)buffer->BITS_255.b242);
  printf("%d", (int)buffer->BITS_255.b243);
  printf("%d", (int)buffer->BITS_255.b244);
  printf("%d", (int)buffer->BITS_255.b245);
  printf("%d", (int)buffer->BITS_255.b246);
  printf("%d", (int)buffer->BITS_255.b247);
  printf("%d", (int)buffer->BITS_255.b248);
  printf("%d", (int)buffer->BITS_255.b249);
  printf("%d", (int)buffer->BITS_255.b250);
  printf("%d", (int)buffer->BITS_255.b251);
  printf("%d", (int)buffer->BITS_255.b252);
  printf("%d", (int)buffer->BITS_255.b253);
  printf("%d", (int)buffer->BITS_255.b254);
  printf("%d", (int)buffer->BITS_255.b255);
  printf("\n");
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
