#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WORD 32
#define HASH_TAB 512

/***
* Do h1 and h2 need to be modulo HASH_TAB or is it enought that
* hash is modulo HASH_TAB?
*
*
***/

struct hashrec{
  char *key;
  int count;
}; 

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

int get_word(char *word, int limit){
  int c;
  char *w = word;

  while (isspace(c = fgetc(stdin))) /* Skip white space */
    ;

  if (EOF != c){
    *w++ = c;
  } else {
    *w = '\0';
    return c; 
  }

  for (; --limit > 0 ; w++)
    if (!isalpha(*w = fgetc(stdin))){
      ungetc(*w, stdin);
      break;
    }
  *w = '\0';
  return word[0];
}

char *cpy(char *w){
  char *d, *dest;

  d = dest = malloc(strlen(w)+1);
  if (d)
    while ((*d++ = *w++)) ;
  return dest;
}

unsigned int locate(struct hashrec hashtab[], char *k){
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

int insert_word(struct hashrec hashtab[], char *k){
  unsigned int b = locate(hashtab, k);

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

int is_word(struct hashrec hashtab[], char *k){
  unsigned int b = locate(hashtab, k);

  if (NULL == hashtab[b].key)
    return 0;
  else 
    return strcmp(hashtab[b].key, k) == 0;
}

int main(){
  int b;
  char word[MAX_WORD];
  struct hashrec wordtab[HASH_TAB];

  for (b = 0 ; b < HASH_TAB ; b++){
    wordtab[b].key = NULL;
    wordtab[b].count = 0;
  }

  while (get_word(word, MAX_WORD) != EOF){
    insert_word(wordtab, word);
  }

  for (b = 0 ; b < HASH_TAB ; b++)
    if (wordtab[b].key){
      printf("%s %d\n", wordtab[b].key, wordtab[b].count);
      printf("is word? %d\n", is_word(wordtab, wordtab[b].key));
    }

  return 0;
}
