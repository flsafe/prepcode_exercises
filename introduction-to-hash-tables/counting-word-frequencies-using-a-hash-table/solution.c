#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WORD 32
#define HASH_TAB 512

struct hashrec{
  char *key;
  int count;
} hashtab[HASH_TAB]; 

unsigned int h1(char *k){
  unsigned h;
  unsigned char *c;
 
  h = 0;
  for(c = (unsigned char*) k; *c ; c++)
    h = h * 37 + *c;
  return h;
}

unsigned int h2(char *k){
  unsigned int h;
  unsigned char *c;
  
  h = 0;
  for(c = (unsigned char*) k ; *c ; c++)
    h = h * 31 + *c;
  return h % 2 == 0 ? h + 1 : h;
}

unsigned hash(char *k, int i){
  return (h1(k) + i * h2(k)) % HASH_TAB;
}

char *cpy(char *w){
  char *d, *dest;

  d = dest = malloc(strlen(w)+1);
  if (d)
    while ((*d++ = *w++)) ;
  return dest;
}

unsigned int locate(char *k){
  int i;
  unsigned int b;

  for (i = 0 ; i < HASH_TAB ; i++){
    b = hash(k, i);
    if (NULL == hashtab[b].key ||
        strcmp(hashtab[b].key, k) == 0)
      break;
  }
  return b;
}

int insert_word(char *k){
  unsigned int b = locate(k);

  if (NULL == hashtab[b].key){
    hashtab[b].key = cpy(k);
    hashtab[b].count++;
    return 1;
  }
  else if (strcmp(hashtab[b].key, k) == 0){
    hashtab[b].count++;
    return 1;
  }
  else
    return 0;
}

void count_words(char *words[], int n){
  int i;
  
  for (i = 0 ; i < n ; i++)
    insert_word(words[i]);
}

void print(){
  int b;

  printf("***\n");
  for (b = 0 ; b < HASH_TAB ; b++)
    if (hashtab[b].key)
      printf("%s %d\n", hashtab[b].key, hashtab[b].count);
}

void clear(){
  int b;

  for (b = 0 ; b < HASH_TAB ; b++){
    hashtab[b].key = NULL;
    hashtab[b].count = 0;
  }
}

int main(){
  char *input1[] = {"at", "at", "at", "sat", "cat", "cat", "mat", "on", "the"};
  char *input2[] = {"programming"};
  char *input3[] = {"a", "b", "c"};
  char *input4[] = {"a", "a", "a", "a", "a"};

  count_words(input1, sizeof(input1)/sizeof(char*));
  print();
  clear();
  count_words(input2, sizeof(input2)/sizeof(char*));
  print();
  clear();
  count_words(input3, sizeof(input3)/sizeof(char*));
  print();
  clear();
  count_words(input4, sizeof(input4)/sizeof(char*));
  print();
  clear();

  return 0;
}
