/*start_prototype*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WORD 32    /* Max word size   */
#define HASH_TAB 512   /* Hash table size */

struct hashrec{        /* Represents a record in the hash table */
  char *key;
  int count;
}; 

/* 
We've provided you with this function that
gets a word from standard input.

  Parameters:
    word  -  Used at output, places the next word 
             from from stdin into this buffer.

    limit -  Will store at most this many characters 
             into the word buffer, including the null character.

  Example usage:

    while (get_word(word, MAX_WORD) != EOF){
      do_something(word);
    }
*/
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

void count_words(){
  char word[MAX_WORD];              /* Holds a word read from stdin */
  struct hashrec wordtab[HASH_TAB]; /* Hash table                   */

  /* Your code goes here */

}
/*end_prototype*/

int main(){
  count_words();
  return 0;
}
