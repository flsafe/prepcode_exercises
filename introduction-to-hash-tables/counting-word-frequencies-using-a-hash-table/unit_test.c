/*start_prototype*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WORD 32    /* Max word size   */
#define HASH_TAB 512   /* Hash table size */

struct hashrec{        /* Represents a record in the hash table */
  char *key;
  int count;
};

struct hashrec hashtab[HASH_TAB]; /* A hash table has been declared for you! */

void count_words(char *words[], int n){

  /* Your code goes here. Note that a
     hashtable has been declared for you! */

}
/*end_prototype*/

int main(){
  char *words[] = {"at", "at", "at", "sat", "cat", "cat", "mat", "on", "the"};
  count_words(words, (int) sizeof(words)/sizeof(char*));

  return 0;
}
