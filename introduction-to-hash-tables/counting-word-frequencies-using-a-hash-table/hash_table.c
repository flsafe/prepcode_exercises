#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define HASH_TAB 512

struct hashrec {
  char *word; 
};

unsigned int h1(char *word){
  unsigned int hash;
  char *c;
  
  c = word;
  for(hash = 0 ; *c ; c++)
    hash = hash * 37 + *c;
  return hash;
}

unsigned int h2(char *word){
  unsigned int hash;
  
  hash = h1(word) + 1;
  if (hash % 2 == 0)
    return hash + 1;
  else
    return hash;
}

unsigned int hash(char *word, int i){
  return (h1(word) + i * h2(word)) % HASH_TAB;
}

char *strdup(char *w){
  char *d, *dest;

  d = dest = malloc(strlen(w)+1);
  while ((*d++ = *w++)) ;
  return dest;
}

int locate(struct hashrec hashtab[], char *word){
  int i, h;

  for (i = 0 ; i < HASH_TAB ; i++){
    h = hash(word, i);
    if (NULL == hashtab[h].word ||
        strcmp(hashtab[h].word, word) == 0)
      break;
  }
  return h;
}

int member(struct hashrec hashtab[], char *word){
  int i = locate(hashtab, word);
  if (NULL == hashtab[i].word) 
    return 0;
  else
    return strcmp(hashtab[i].word, word) == 0;
}

int insert(struct hashrec hashtab[], char *word){
  int i;

  if (member(hashtab, word))
    return 1;

  i = locate(hashtab, word);
  if (hashtab[i].word == NULL){
    hashtab[i].word = strdup(word);
    return 1;
  } 
  else
    return 0;
}

void init_hash_table(struct hashrec hashtab[]){
  int i;

  for (i = 0 ; i < HASH_TAB ; i++)
    hashtab[i].word = NULL;
}

#define MAX_WORD 32

int main(){
  struct hashrec hashtab[HASH_TAB];
  char *set1[] = {"apple", "grape", "cherry", "orange", "apple"}; 
  size_t i;

  init_hash_table(hashtab); 
  for (i = 0 ; i < sizeof(set1)/sizeof(char*) ; i++){
    if (!member(hashtab, set1[i]))
      insert(hashtab, set1[i]);
    else{
      printf("Duplicate: %s\n", set1[i]);
      break;
    }
  }
  return 0;
}
