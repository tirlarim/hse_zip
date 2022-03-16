#ifndef HSE_ZIP_UNION_H
#define HSE_ZIP_UNION_H

#define BIT8 8

typedef union bit_to_char {
  unsigned char symbol;
  struct bit {
    unsigned b1 : 1;
    unsigned b2 : 1;
    unsigned b3 : 1;
    unsigned b4 : 1;
    unsigned b5 : 1;
    unsigned b6 : 1;
    unsigned b7 : 1;
    unsigned b8 : 1;
  }mbit;
}BIT_TO_CHAR;

#endif //HSE_ZIP_UNION_H
