#ifndef HSE_ZIP_UNION_H
#define HSE_ZIP_UNION_H

#define BIT8 8

typedef union bit_to_char {
  unsigned char character;
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

typedef union char_to_bits {
  unsigned char characters[3];
  struct bits_19 {
    unsigned b1 : 1;
    unsigned b2 : 1;
    unsigned b3 : 1;
    unsigned b4 : 1;
    unsigned b5 : 1;
    unsigned b6 : 1;
    unsigned b7 : 1;
    unsigned b8 : 1;
    unsigned b9 : 1;
    unsigned b10 : 1;
    unsigned b11 : 1;
    unsigned b12 : 1;
    unsigned b13 : 1;
    unsigned b14 : 1;
    unsigned b15 : 1;
    unsigned b16 : 1;
    unsigned b17 : 1;
    unsigned b18 : 1;
    unsigned b19 : 1;
//    unsigned b20 : 1;
//    unsigned b21 : 1;
//    unsigned b22 : 1;
//    unsigned b23 : 1;
//    unsigned b24 : 1;
  }BITS_20;
}CHAR_TO_BITS;

#endif //HSE_ZIP_UNION_H
