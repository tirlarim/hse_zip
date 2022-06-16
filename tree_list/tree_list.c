#include "tree_list.h"
#include "union.h"
#include <time.h>

long long* init_array_with_zeroes(long long count);
void get_chars_frequency(char filename[], long long* freq_arr, unsigned long long* length);
void add_to_list (NODE** init, unsigned long long freq, unsigned char symbol, NODE* branch);
void make_list(NODE** init, long long* freq_arr);
void make_tree(NODE** init);
void print_tree_on_side(const NODE* init, long long level);
void create_codes(NODE** init, long long level, char* temp_code);
void symmetric(NODE* init, FILE* file, TRIPLE* arr);
void find_and_copy_code(NODE** init, char** code_array, long long symbol);
void change_symbols_to_codes(char input_filename[], char output_filename[], long length, NODE** init);
void archive(char input_filename[], char output_filename[], long length, NODE** init);
void decode(char* fileNameOutput);
void printTreeCodes(const NODE* init);
void prepareBytesBuffer(int* buffCode, FILE* fp, int* iRead, int arrSize, unsigned long* fileLen);
char findAnswer(CODES_AS_TREE* root, const int* arrayLen, long long* offset, int readIndex);
void fillArrMinusOne(int* arr);
CODES_AS_TREE* Add2Tree(CODES_AS_TREE* root, int arrayLen, int deepIndex, int* arr, char value);

void init_tree(NODE* init, char* fileNameInput, char* fileNameOutput) {
  clock_t startTime, endTime;
  startTime = clock();
  long long* freq = init_array_with_zeroes(SYMBOLS_COUNT); //symbols frequency
  if (PRINTF_DEBUG) printf("init_array_with_zeroes - DONE.\n");
  unsigned long long length = 0; //symbols count
  get_chars_frequency(fileNameInput,freq, &length);
  if (PRINTF_DEBUG) printf("get_chars_frequency - DONE.\n");
  make_list(&init, freq);
  if (PRINTF_DEBUG) printf("make_list - DONE.\n");
  free(freq);
  if (PRINTF_DEBUG) printf("free - DONE.\n");
  make_tree(&init);
  if (PRINTF_DEBUG) printf("make_tree - DONE.\n");
  char* temp_code = (char*)malloc(CODE_SIZE*sizeof(char));
  if (PRINTF_DEBUG) printf("temp_code_init - DONE.\n");
  create_codes(&init, 0, temp_code);
  if (PRINTF_DEBUG) printf("create_codes - DONE.\n");
  change_symbols_to_codes(fileNameInput, filename_buffer, length, &init); //write 10101.. to buffer.txt
  if (PRINTF_DEBUG) printf("change_symbols_to_codes - DONE.\n");
  archive(fileNameInput, fileNameOutput, length, &init); //take codes from buffer.txt and unite them
  if (PRINTF_DEBUG) printf("archive - DONE.\n");
  endTime = clock();
  printf("archive time: %.2lf sec.\n", (double)(endTime - startTime) / (CLOCKS_PER_SEC));
  decode(fileNameOutput);
  if (PRINTF_DEBUG) printf("decode - DONE.\n");
}

//debug
void print_tree_on_side(const NODE* init, long long level) {
  if (init) {
    print_tree_on_side(init->right, level + 1);
    for (long long i = 0; i < level; i++) {
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

void symmetric(NODE* init, FILE* file, TRIPLE* arr) {
  if (init) {
    symmetric(init->left, file, arr);
    if (init->is_symbol) {
      arr[(int) init->symbol].symbol = init->symbol; //array for print symbols in frequency order
      arr[(int) init->symbol].freq = init->freq;
      strcpy(arr[(int) init->symbol].code, init->code);
    }
    symmetric(init->right, file, arr);
  }
}

void create_codes(NODE** init, long long level, char* temp_code) {
  if (*init) {
    if ((*init)->is_symbol != 0) {
      if (!level && !((*init)->next)) {
        (*init)->code[0] = '1';
        (*init)->code[1] = '\0';
        return;
      }
      temp_code[level] = '\0';
      strcpy((*init)->code, temp_code);
    }
    temp_code[level] = '0';
    create_codes(&((*init)->left), level + 1, temp_code);
    temp_code[level] = '1';
    create_codes(&((*init)->right), level + 1, temp_code);
  }
}

void find_and_copy_code(NODE** init, char** code_array, long long symbol) {
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
      for (long long i = 0; i < CODE_SIZE; ++i) {
        printf("%d", init->code[i]);
      }
      printf("\n");
    }
    printTreeCodes(init->left);
  }
}

long long* init_array_with_zeroes(long long count) {
  long long* arr = (long long*)malloc(count*sizeof(long long));
  for (long long i = 0; i < count; i++) {
    arr[i] = 0;
  }
  return arr;
}

void get_chars_frequency(char filename[], long long* freq_arr, unsigned long long* length) {
  FILE* input = fopen(filename, "rb");
  if (!input) exit(2);
  fseek(input, 0, SEEK_END);
  *length = ftell(input);
  fseek(input, 0, SEEK_SET);
  if (PRINTF_DEBUG) printf("get file size: %llu - DONE.\n", *length);
  unsigned char buffer[BUFFER_SIZE];
  long long first_time = 1;
  unsigned long long count = *length;
  unsigned long long bytes_read = 0;
  while (first_time || count > 0) {
    first_time = 0;
    count -= BUFFER_SIZE;
    bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, input);
    for (long long i = 0; i < bytes_read; i++) {
      freq_arr[(int) buffer[i]]++;
    }
  }
  fclose(input);
}

void add_to_list(NODE** init, unsigned long long freq, unsigned char symbol, NODE* branch) {
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

void make_list(NODE** init, long long* freq_arr) {
  for (long long i = 0; i < SYMBOLS_COUNT; i++) {
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
  for (long long i = 0; i < 256; i++) {
    codes_array[i] = (char*)malloc(256*sizeof(char));
  }
  for (long long i = 0; i < 256; i++) {
    find_and_copy_code(init, codes_array, i);
  }
  FILE* input = fopen(input_filename, "rb");
  FILE* output = fopen(output_filename, "w+");
  unsigned char buffer[BUFFER_SIZE];
  long long first_time = 1;
  while (length > 0 || first_time) {
    length -= BUFFER_SIZE;
    first_time = 0;
    unsigned long long read_bytes = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, input);
    for (long long i = 0; i < read_bytes; i++) {
      fprintf(output, "%s", codes_array[(int)buffer[i]]);
    }
  }
  free(codes_array);
  fclose(input);
  fclose(output);
}

void archive(char input_filename[], char output_filename[], long length, NODE** init) {
  FILE* get_codes = fopen(filename_buffer, "rb");
  fseek(get_codes, 0, SEEK_END);
  long count = ftell(get_codes);
  fseek(get_codes, 0, SEEK_SET);

  FILE* final = fopen(output_filename, "wb");
  TRIPLE* freq_array = (TRIPLE*)malloc(256*sizeof(TRIPLE));
  for (long long i = 0; i < 256; i++) {
    freq_array[i].freq = 0;
  }
  symmetric(*init, final, freq_array);//get symbol, freq and code to array of TRIPLE
  for (int i = 0; i < 256; i++) { //bubble sort
    for (int j = i + 1; j < 256; j++) {
      if (freq_array[j].freq > freq_array[i].freq) {
        TRIPLE temp = freq_array[i];
        freq_array[i] = freq_array[j];
        freq_array[j] = temp;
      }
    }
  }

  for (int i = 0; i < 256; i++) { //print symbols and codes
    if (freq_array[i].freq == 0)
      break;
    fprintf(final, "%c:%s", freq_array[i].symbol, freq_array[i].code);
  }

  fprintf(final, "\n\n%ld", length);
  long long filename_pointer = (long long) strlen(input_filename);
  filename_pointer--;
  for (; filename_pointer > 0; filename_pointer--) {
    if (input_filename[filename_pointer] == '/') {
      filename_pointer++; //to find beginning of filename
      break;
    }
  }

  fprintf(final, "\n%s\n", input_filename + filename_pointer);
  unsigned long long bytes_read = 0;
  unsigned char buffer[BUFFER_SIZE];
  memset(buffer, '0', BUFFER_SIZE);
  long long first_time = 1;
  while (count > 0 || first_time) {
    first_time = 0;
    count -= BUFFER_SIZE;
    memset(buffer, '0', bytes_read);
    bytes_read = fread(buffer, sizeof(unsigned char), BUFFER_SIZE, get_codes);
    long long i = 0;
    long long buffer_pointer = 0;
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
  free(freq_array);
  fclose(get_codes);
  fclose(final);
  remove(filename_buffer);
}

void printProgress(double percentage, unsigned long long sec) {
  int val = (int) (percentage * 100);
  int leftPad = (int) (percentage * PBWIDTH);
  int rightPad = PBWIDTH - leftPad;
  sec = percentage ? sec : 0;
  printf("\r%3d%% [%.*s%*s] est. time: ~ ", val, leftPad, PBSTR, rightPad, "");
  if (sec >= 604800) {
    printf("%llu weeks %lld days %lld hours %lld minutes %lld sec", sec/604800, sec/604800/86400, sec/604800/86400/3600, sec/604800/86400/3600/60, sec%60);
  } else if (sec >= 86400) {
    printf("%lld days %lld hours %lld minutes %lld sec", sec/86400, sec/86400/3600, sec/86400/3600/60, sec%60);
  } else if (sec >= 3600) {
    printf("%lld hours %lld minutes %lld sec", sec/3600, sec/3600/60, sec%60);
  } else if (sec >= 60) {
    printf("%lld minutes %lld sec", sec/60, sec%60);
  } else if (sec < 60) {
    printf("%lld sec", sec%60);
  }
  fflush(stdout);
}

void decode(char* fileNameOutput) {
  clock_t startTime, endTime;
  startTime = clock();
  char header[BYTES_COUNT*CODE_SIZE] = {0};
  unsigned char headerSorted[BYTES_COUNT] = {0}; int headerSortedIndex = 0;
  char outputFileName[1000] = "../testDataOutput/";
  int ansIndex = 0;
  unsigned long fileNameLength;
  char decodeFileName[1000] = {0};
  unsigned long long decodeFileSizeBytes = 0;
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
  unsigned long long length = ftell(output);
  fseek(output, 0, SEEK_SET);
  fclose(output);
  if (PRINTF_DEBUG) printf("decode -> get file size: %llu -> DONE.\n", length);
//  read header
  FILE* final = fopen(fileNameOutput, "rb");
  for (int i = 0; i < BYTES_COUNT * CODE_SIZE; ++i) {
    header[i] = (char)getc(final);
    if (header[i-1] == '\n' && header[i] == '\n') {
      header[i-1] = '\0'; header[i] = '\0';
      break;
    }
  }
  if (PRINTF_DEBUG) printf("decode -> read header -> DONE.\n");
//  create a table
  for (int i = 1; header[i] != '\0'; ++i) {
    unsigned char byte;
    if (header[i-1] == ':' && (header[i] == 48 || header[i] == 49)) {
      byte = header[i-2];
      headerSorted[headerSortedIndex++] = byte;
      for (int j = 0;(header[i] == 48 || header[i] == 49) && (header[i+1] != ':' || (header[i+1] == ':' && header[i+2] == ':')); ++i) {
        codes[byte][j++] = header[i] - '0';
      }
    }
    length--;
  }
  if (PRINTF_DEBUG) printf("decode -> create a table -> DONE.\n");
  CODES_AS_TREE* root = NULL;
  root = (CODES_AS_TREE*)malloc(sizeof(CODES_AS_TREE));
  root->right=NULL;
  root->left=NULL;
  int arrLen[256];
  memset(arrLen, 0, sizeof(arrLen));
  for (int i = 0; i < 256; ++i) {
    for (int j = 0; codes[(int)headerSorted[i]][j] != -1 && j < 256; ++j) {
      arrLen[i] = j+1;
    }
  }
  for (int i = 0; i < 256; ++i) {
    int arrCurrentLen = 0;
    if (codes[(int)headerSorted[i]][0] != -1) {
      root = Add2Tree(root, arrLen[i], arrCurrentLen, codes[(int)headerSorted[i]], (char)headerSorted[i]);
    }
  }
  if (PRINTF_DEBUG) printf("decode -> create a bin tree -> DONE.\n");
  length-=5;
  fscanf(final, "%lld\n",  &decodeFileSizeBytes); //  get bits count
  if (PRINTF_DEBUG) printf("decode -> get decodeFileSizeBytes: %lld -> DONE.\n", decodeFileSizeBytes);
  fscanf(final, "%s\n",  decodeFileName); //  get name
  if (PRINTF_DEBUG) printf("decode -> get decodeFileName: %s -> DONE.\n", decodeFileName);
  fileNameLength = strlen(decodeFileName);
  length -= fileNameLength;
  char ans[1000+1];
  printf("start decode\n");
  int readIndex = 0;
  int arrSize = 80000; // this isn't limit. we can create arr bigger
  int buffCode[8*10000] = {0}; //arrSize = buffCode.length
  long long lastOffset = 0;
  unsigned long long decodeBits = 0;
  unsigned int onePercentOfFile = decodeFileSizeBytes/100;
  unsigned long fileCurrentLen = length;
  BIT_TO_CHAR symbol;
  fillArrMinusOne(buffCode);
  if (PRINTF_DEBUG) printf("decode -> fillArrMinusOne -> DONE.\n");
  for (int i = 0; i < arrSize/8 && i < length; ++i) {
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
  if (PRINTF_DEBUG) printf("decode -> first 80k read -> DONE.\n");
  strncat(outputFileName, decodeFileName, sizeof(outputFileName) - fileNameLength - 1);
  FILE *fp = fopen(outputFileName, "wb" );
  clock_t loopStart, loopEnd;
  loopStart = clock();
  while (decodeBits < decodeFileSizeBytes) { // start
    if (decodeBits % onePercentOfFile == 0 && onePercentOfFile != 0) {
      loopEnd = clock();
      double progress = ((double)decodeBits / (double)decodeFileSizeBytes) + 0.01;
      printProgress(progress,(unsigned long long)(((double)(loopEnd - loopStart) / (CLOCKS_PER_SEC))*(((double)1-progress)*100)));
      loopStart = clock();
    }
    lastOffset = 0;
    ans[ansIndex++] = findAnswer(root, buffCode, &lastOffset, readIndex);
    readIndex+=(int)lastOffset;
    if (decodeBits % 1000 == 0 || decodeBits == decodeFileSizeBytes - 1) {
      fwrite(ans , 1, ansIndex, fp);
      ansIndex = 0;
      memset(ans, 0, sizeof(ans));
    }
    if (readIndex>(arrSize-256) && fileCurrentLen) {
      prepareBytesBuffer(buffCode, final, &readIndex, arrSize, &fileCurrentLen);
    }
    decodeBits++;
  }
  fclose(fp);
  endTime = clock();
  printf("\n");
  printf("decode time: %.2lf sec.\n", (double)(endTime - startTime) / (CLOCKS_PER_SEC));
}

void prepareBytesBuffer(int* buffCode, FILE* fp, int* iRead, int arrSize, unsigned long* fileLen) {
  int trashCount = 0;
  for (int i = arrSize-1; buffCode[i] == -1 && i > arrSize-256; --i) {
    trashCount++; // ~10-15 iterations
  }
  for (int i = *iRead; i < arrSize && buffCode[i] != -1; ++i) {
    buffCode[i-*iRead] = buffCode[i]; //~256 iterations
  }
  *iRead = arrSize-*iRead-trashCount;
  for (int i = arrSize-1; i > arrSize-30; --i) {
    buffCode[i] = -1; //~30 iterations
  }
  BIT_TO_CHAR symbol;
  for (int i = 0; i < (arrSize - *iRead)/8; ++i) {
    symbol.character = (unsigned char)getc(fp); // fix it
    buffCode[*iRead+8*i+0] = (int)symbol.mbit.b1;
    buffCode[*iRead+8*i+1] = (int)symbol.mbit.b2;
    buffCode[*iRead+8*i+2] = (int)symbol.mbit.b3;
    buffCode[*iRead+8*i+3] = (int)symbol.mbit.b4;
    buffCode[*iRead+8*i+4] = (int)symbol.mbit.b5;
    buffCode[*iRead+8*i+5] = (int)symbol.mbit.b6;
    buffCode[*iRead+8*i+6] = (int)symbol.mbit.b7;
    buffCode[*iRead+8*i+7] = (int)symbol.mbit.b8;
    --*fileLen;
  }
  *iRead=0;
}

void fillArrMinusOne(int arr[256*2]) {
  int arrLen = 256*2;
  for (int i = 0; i < arrLen; ++i) {
    arr[i] = -1;
  }
}

char findAnswer(CODES_AS_TREE* root, const int* arrayLen, long long* offset, int readIndex) {
  if (root->is_symbol==true) {
    return root->symbol;
  } else {
    if (arrayLen[*offset+readIndex] == 1) {
      ++(*offset);
      return findAnswer(root->right, arrayLen, offset, readIndex);
    } else {
      ++(*offset);
      return findAnswer(root->left, arrayLen, offset, readIndex);
    }
  }
}


CODES_AS_TREE* Add2Tree(CODES_AS_TREE* root, int arrayLen, int deepIndex, int* arr, char value) {
  if (root == NULL) {
    root = (CODES_AS_TREE*)malloc(sizeof(CODES_AS_TREE));
  }
  if (deepIndex == arrayLen) {
    root->symbol = value;
    root->is_symbol = true;
  }
  if (deepIndex != arrayLen && arr[deepIndex] == 0) {
    root->left = Add2Tree(root->left, arrayLen, ++deepIndex, arr, value);
    return root;
  }
  if (deepIndex != arrayLen && arr[deepIndex] == 1) {
    root->right = Add2Tree(root->right, arrayLen, ++deepIndex, arr, value);
    return root;
  }
  return root;
}