#include <stdio.h>

#define STRLEN 5

void to_bin(int i, char *str, int lim){
  int j;

  str[--lim] = '\0';
  for (j = 0 ; j < lim ; j++)
    str[j] = '0';

  for (; --lim > 0 && i ; i = i >> 1)
    str[lim] = i & 0x1 ? '1' : '0';
}

int main(int argc, char **argv){
  int i;
  char mod[STRLEN];
  char mask[STRLEN];

  for (i = 0 ; i < 32 ; i++){
    to_bin(i%32, mod, STRLEN);
    to_bin(i&0x1f, mask, STRLEN);
    printf("%5d\t%5d\t%5s\t%5s\n", i, i % 32, mod, mask); 
  }

  return 0;
}
