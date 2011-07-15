#include <stdio.h>
#include <ctype.h>

#define MAX_WORD 32

int get_word(char *, int);

void insert_word(char *w);

void count_word_freqs(void){


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
    return word[0];
  }

  for (; --limit > 0 ; w++)
    if (!isalpha(*w = fgetc(stdin))){
      ungetc(*w, stdin);
      break;
    }
  *w = '\0';
  return word[0];
}

int main(){
  char word[MAX_WORD];

  while (get_word(word, MAX_WORD) != EOF){
    printf("%s\n", word);
  }

  return 0;
}
