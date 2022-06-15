#include <string.h>
#include <stdio.h>
#include <time.h>
#include "tree_list/tree_list.h"
#include "tree_list/union.h"


long long* init_array_with_zeroes(long long count);
void get_chars_frequency(char filename[], long long* freq_arr, long* length);
void add_to_list (NODE** init, unsigned long long freq, unsigned char symbol, NODE* branch);
void make_list(NODE** init, long long* freq_arr);
void make_tree(NODE** init);
void print_tree_on_side(const NODE* init, long long level);
void create_codes(NODE** init, long long level, char* temp_code);
void symmetric(NODE* init, FILE* file, TRIPLE* arr);
void find_and_copy_code(NODE** init, char** code_array, long long symbol);
void change_symbols_to_codes(char input_filename[], char output_filename[], long length, NODE** init);
void archive(char input_filename[], char output_filename[], long length, NODE** init);

int main(int argc, char* argv[]) {
  NODE* binTree = NULL;
  char fileNameInput[] = "../testDataInput/text_100byte.txt",
       fileNameOutput[] = "../testDataOutput/answer.hse_zip";
  printf("working on: %s\n", fileNameInput);
  init_tree(binTree, fileNameInput, fileNameOutput);
  return 0;
}

void init_tree(NODE* init, char* fileNameInput, char* fileNameOutput) {
  clock_t startTime, endTime;
  startTime = clock();
  long long* freq = init_array_with_zeroes(SYMBOLS_COUNT); //symbols frequency
  long length = 0; //symbols count
  get_chars_frequency(fileNameInput,freq, &length);
  make_list(&init, freq);
  free(freq);
  make_tree(&init);
  char* temp_code = (char*)malloc(CODE_SIZE*sizeof(char));
  create_codes(&init, 0, temp_code);
  change_symbols_to_codes(fileNameInput, filename_buffer, length, &init); //write 10101.. to buffer.txt
  archive(fileNameInput, fileNameOutput, length, &init); //take codes from buffer.txt and unite them
  endTime = clock();
  printf("archive time: %.2lf sec.\n", (double)(endTime - startTime) / (CLOCKS_PER_SEC));
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

void get_chars_frequency(char filename[], long long* freq_arr, long* length) {
  FILE* input = fopen(filename, "rb");
  if (!input) exit(2);
  fseek(input, 0, SEEK_END);
  *length = ftell(input);
  fseek(input, 0, SEEK_SET);
  unsigned char buffer[BUFFER_SIZE];
  long long first_time = 1;
  long long count = *length;
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