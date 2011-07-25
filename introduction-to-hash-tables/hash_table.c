#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASH_TAB 512

struct hashrec {
  char *key; 
};

unsigned int h1(char *k){
  unsigned int h;
  unsigned char *c;
  
  h = 0;
  for (c = (unsigned char*)k ; *c ; c++)
    h = h * 37 + *c;
  return h;
}

unsigned int h2(char *k){
  unsigned int h;
  unsigned char *c;

  for (c = (unsigned char*)k ; *c ; c++)
    h = h * 31 + *c;
  return h % 2 == 0 ? h + 1 : h;
}

unsigned int hash(char *k, int i){
  return (h1(k) + i * h2(k)) % HASH_TAB;
}

char *cpy(char *s){
  char *dest, *d;

  d = dest = malloc(strlen(s) * sizeof(char) + 1);
  if (d)
    while ((*d++ = *s++)) ; /* Copy */
  return dest;
}

int locate(struct hashrec hashtab[], char *k){
  unsigned int i, b;

  for (i = 0 ; i < HASH_TAB ; i++){
    b = hash(k, i);
    if (NULL == hashtab[b].key ||
        strcmp(hashtab[b].key, k) == 0)
      break;
  }
  return b;
}

int member(struct hashrec hashtab[], char *key){
  unsigned int b = locate(hashtab, key);
  if (NULL == hashtab[b].key) 
    return 0;
  else
    return strcmp(hashtab[b].key, key) == 0;
}

int insert(struct hashrec hashtab[], char *key){
  unsigned int b;

  b = locate(hashtab, key);
  if (NULL == hashtab[b].key){
    hashtab[b].key = cpy(key);
    return 1;
  }
  else
    return strcmp(hashtab[b].key, key) == 0;
}

void init_hash_table(struct hashrec hashtab[]){
  unsigned int b;

  for (b = 0 ; b < HASH_TAB ; b++)
    hashtab[b].key = NULL;
}

int main(){
  struct hashrec hashtab[HASH_TAB];
  char *set[] = {"grape", "cherry", "orange", "grape", "apple"}; 
  size_t i;

  init_hash_table(hashtab); 
  for (i = 0 ; i < sizeof(set)/sizeof(char*) ; i++){
    if (!member(hashtab, set[i]))
      insert(hashtab, set[i]);
    else{
      printf("Duplicate: %s\n", set[i]);
      break;
    }
  }
  return 0;
}
