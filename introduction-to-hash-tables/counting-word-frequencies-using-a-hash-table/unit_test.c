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

struct wordrec {
  char *word; 
  int count;
} wordtab[HASH_TAB];

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
  char *c;
  
  c = word;
  for(hash = 0 ; *c ; c++)
    hash = hash * 31 + *c;
  if (hash % 2 == 0)
    return hash + 1;
  else
    return hash;
}

unsigned hash(char *word, int i){
  return (h1(word) + i * h2(word)) % HASH_TAB;
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

char *strdup(char *w){
  char *d, *dest;

  d = dest = malloc(strlen(w)+1);
  while ((*d++ = *w++)) ;
  return dest;
}

int insert_word(char *word){
  int i;
  int h;
  
  for (i = 0 ; i < HASH_TAB ; i++){
    h = hash(word, i);
    if (NULL == wordtab[h].word){
      wordtab[h].word = strdup(word);
      wordtab[h].count++;
      return 1;
    } else if (strcmp(wordtab[h].word, word) == 0){
      wordtab[h].count++; 
      return 1;
    }
  }
  return 0;
}

int is_word(char *word){
  int i, h;

  for (i = 0 ; i < HASH_TAB ; i++){
    h = hash(word, i);
    if (strcmp(word, wordtab[h].word) == 0)
      return 1;
  }
  return 0;
}

void count_word_freqs(void){
  int i;
  char word[MAX_WORD];

  while (get_word(word, MAX_WORD) != EOF){
    insert_word(word);
  }

  for (i = 0 ; i < HASH_TAB ; i++)
    if (NULL != wordtab[i].word){
      printf("%s %d\n", wordtab[i].word, wordtab[i].count);
      printf("is word? %d\n", is_word(wordtab[i].word));
    }
}

int main(){
  count_word_freqs();
  return 0;
}
