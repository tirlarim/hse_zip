#include <stdio.h>

int main() {
  FILE *fp;
  int a = 0;
  fp = fopen("../testData/example.txt","rb");
  fseek(fp, 0, SEEK_END);
  long int size = ftell(fp); // now you got size of file in bytes
  fseek(fp, 0, SEEK_SET);    // same as rewind(fp)
  for (int i = 0; i < size; i++) {
    fread(&a,sizeof(int),1,fp); // you read one int (sizeof(int)!=1 byte)
    if (i % 5 == 0 && i != 0) {
      printf("\n");
    }
    printf("0x%x\t", a);
  }
  printf("\n");
  printf("Size of file: %ld bytes.", size);
  fclose(fp);
  return 0;
}
