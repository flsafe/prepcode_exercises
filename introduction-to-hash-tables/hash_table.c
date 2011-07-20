#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASH_TAB 512

struct hashrec {
  char *key; 
};

unsigned int h1(char *key){
  unsigned int hash;
  char *c;
  
  c = key;
  for(hash = 0 ; *c ; c++)
    hash = hash * 37 + *c;
  return hash;
}

unsigned int h2(char *key){
  unsigned int hash;
  
  hash = h1(key) + 1;
  if (hash % 2 == 0)
    return hash + 1;
  else
    return hash;
}

unsigned int hash(char *key, int i){
  return (h1(key) + i * h2(key)) % HASH_TAB;
}

char *cpy(char *w){
  char *d, *dest;

  d = dest = malloc(strlen(w)+1);
  while ((*d++ = *w++)) ;
  return dest;
}

int locate(struct hashrec hashtab[], char *key){
  int i, h;

  for (i = 0 ; i < HASH_TAB ; i++){
    h = hash(key, i);
    if (NULL == hashtab[h].key ||
        strcmp(hashtab[h].key, key) == 0)
      break;
  }
  return h;
}

int member(struct hashrec hashtab[], char *key){
  int b = locate(hashtab, key);
  if (NULL == hashtab[b].key) 
    return 0;
  else
    return strcmp(hashtab[b].key, key) == 0;
}

int insert(struct hashrec hashtab[], char *key){
  int b;

  if (member(hashtab, key))
    return 1;

  b = locate(hashtab, key);
  if (NULL == hashtab[b].key){
    hashtab[b].key = cpy(key);
    return 1;
  } 
  else
    return 0;
}

void init_hash_table(struct hashrec hashtab[]){
  int b;

  for (b = 0 ; b < HASH_TAB ; b++)
    hashtab[b].key = NULL;
}

int main(){
  struct hashrec hashtab[HASH_TAB];
  char *set[] = {"apple", "grape", "cherry", "orange", "apple"}; 
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
