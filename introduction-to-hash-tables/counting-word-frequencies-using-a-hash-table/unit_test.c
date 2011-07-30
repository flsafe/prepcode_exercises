/*start_prototype*/
#define HASH_TAB 512

struct hashrec{
  char *key;
  int count;
} hashtab[HASH_TAB]; 

void count_words(char *words[], int n){
 
  /*
     You code goes here!
     Notice that the hash table has been
     declared for you above.
  */ 

}
/*end_prototype*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* The maxium length of the YAML output string */
#define MAX_YAML_LENaab 10000 

/* The indentation in spaces in the yaml string */
#define INDENTaab 4

/* Max output of function under test */
#define MAX_OUT_BUFF 10000 

#define MAX_WORD 32

struct yaml_stringaab{  
  char yaml[MAX_YAML_LENaab];
  int write;
};
struct yaml_stringaab * create_yamlaab(void);
void print_test_infoaab(struct yaml_stringaab * ys,
                     char * name,
                     char * input, 
                     char * expected, 
                     char * points);
void print_test_outputaab(struct yaml_stringaab * ys, char * str);

int cmp(const void *p1, const void *p2){
  struct hashrec *r1 = (struct hashrec *) p1;
  struct hashrec *r2 = (struct hashrec *) p2;

  if (r1->key == NULL || r2->key == NULL)
    return r1->key - r2->key;
  else
    return strcmp(r1->key, r2->key);
}

void print_hashtab(char *out, unsigned int max){
  int b;
  char line[MAX_WORD];

  for (b = 0 ; b < HASH_TAB ; b++)
    if (hashtab[b].key){
      snprintf(line, MAX_WORD, "%s %d\n", hashtab[b].key, hashtab[b].count);
      if (strlen(line) + strlen(out) < max)
        strcat(out, line);
      else
        break;
    }
}

void clear_hashtab(){
  int b;

  for (b = 0 ; b < HASH_TAB ; b++){
    hashtab[b].key = NULL;
    hashtab[b].count = 0;
  }
}

void test_multiple_words(void){
  char buff[MAX_OUT_BUFF] = "";
  char *words[] = {"the", "cat", "the", "hat", "the", "mat"};
  struct yaml_stringaab * ys = create_yamlaab();

  print_test_infoaab(ys, "test_multiple_words", 
                      "the cat the hat the mat", 
                      "cat 1\nhat 1\nmat 1\nthe 3", 
                      "20");

  clear_hashtab();
  count_words(words, sizeof(words)/sizeof(char*));

  qsort(hashtab, HASH_TAB, sizeof(struct hashrec), cmp);
  print_hashtab(buff, MAX_OUT_BUFF);
  print_test_outputaab(ys, buff); 
}

void test_one_word(void){
  char buff[MAX_OUT_BUFF] = "";
  char *words[] = {"programming"};
  struct yaml_stringaab * ys = create_yamlaab();

  print_test_infoaab(ys, "test_one_word", "programming", "programming 1", "20");
  
  clear_hashtab();
  count_words(words,sizeof(words)/sizeof(char*));

  qsort(hashtab, HASH_TAB, sizeof(struct hashrec), cmp);
  print_hashtab(buff, MAX_OUT_BUFF);
  print_test_outputaab(ys, buff); 
}

void test_unique_words(void){
  char buff[MAX_OUT_BUFF] = "";
  char *words[] = {"cat", "sat", "on", "the", "mat"};
  struct yaml_stringaab * ys = create_yamlaab();

  print_test_infoaab(ys, "test_unique_words", 
                      "cat sat on the mat", 
                      "cat 1\nmat 1\non 1\nsat 1\nthe 1", 
                      "20");

  clear_hashtab();
  count_words(words, sizeof(words)/sizeof(char*));

  qsort(hashtab, HASH_TAB, sizeof(struct hashrec), cmp);
  print_hashtab(buff, MAX_OUT_BUFF);
  print_test_outputaab(ys, buff); 
}

void test_repeat_word(void){
  char buff[MAX_OUT_BUFF] = "";
  char *words[] = {"a", "a", "a", "a", "a"};
  struct yaml_stringaab * ys = create_yamlaab();

  print_test_infoaab(ys, "test_repeat_word", "a a a a a", "a 5", "20");

  clear_hashtab();
  count_words(words, sizeof(words)/sizeof(char*));

  qsort(hashtab, HASH_TAB, sizeof(struct hashrec), cmp);
  print_hashtab(buff, MAX_OUT_BUFF);
  print_test_outputaab(ys, buff); 
}

void test_zero_words(void){
  char buff[MAX_OUT_BUFF] = "";
  char *words[] = {NULL};
  struct yaml_stringaab * ys = create_yamlaab();

  print_test_infoaab(ys, "test_zero_words", "", "", "20");

  clear_hashtab();
  count_words(words, 0);

  qsort(hashtab, HASH_TAB, sizeof(struct hashrec), cmp);
  print_hashtab(buff, MAX_OUT_BUFF);
  print_test_outputaab(ys, buff); 
}

/**********
*
*  Auto generated code 
*
***********/

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

/* Specifies the number of unit tests */
#define N_UNIT_TESTSaab 5

/* File descriptors for stdin and stdout */
#define INaab 0
#define OUTaab 1

/* Pipe used to communicate with each forked process */
int pipe_desaab[2];

/* Used to ditch when something goes wrong */
void quitifaab(int err){
  if (-1 == err){
    perror("error");
    exit(1);
  }
}

struct yaml_stringaab * create_yamlaab(){
  struct yaml_stringaab * y;
  y = calloc(sizeof(struct yaml_stringaab), sizeof(char));
  y->write = 0;
  return y;
}

void append_lineaab(struct yaml_stringaab * s, char * str, int indent){
  int i,j;
  int len = strlen(str);

  if (s->write + len > MAX_YAML_LENaab)
    return;

  indent = indent < 0 ? 0 : indent;
  for (i = 0 ; i < indent ; i++)
    s->yaml[s->write++] = ' ';

  for (i = 0 ; i < len ; i++){
    s->yaml[s->write++] = str[i];
    if ('\n' == str[i])
      for (j = 0 ; j < indent ; j++)
        s->yaml[s->write++] = ' ';
  }

  s->yaml[s->write++] = '\n';
}

void flush_yamlaab(struct yaml_stringaab * ys){
  int i, bytes_written;

  bytes_written = write(pipe_desaab[OUTaab], ys->yaml, ys->write);
  quitifaab(bytes_written);

  for (i = ys->write - 1 ; i ; i--)
    ys->yaml[i] = '\0';
  ys->write = 0;
}

void print_test_infoaab(struct yaml_stringaab * ys,
    char * name,
    char * input, 
    char * expected, 
    char * points){

  char test_name[256];

  sprintf(test_name, "%s:", name);
  append_lineaab(ys, test_name, 0);
    append_lineaab(ys, "input: |", INDENTaab);
      append_lineaab(ys, input, INDENTaab * 2);

    append_lineaab(ys, "expected: |", INDENTaab);
      append_lineaab(ys, expected, INDENTaab * 2);

    append_lineaab(ys, "points: |", INDENTaab);
      append_lineaab(ys, points, INDENTaab * 2);

  flush_yamlaab(ys);
}

void print_test_outputaab(struct yaml_stringaab * ys, char * str){
  append_lineaab(ys, "output: |", INDENTaab);
    append_lineaab(ys, str, INDENTaab * 2);
  flush_yamlaab(ys);
}

void print_resultsaab(){
  int bytes_read, err;
  char buff[MAX_YAML_LENaab] = "";

  bytes_read = read(pipe_desaab[INaab], buff, MAX_YAML_LENaab);
  quitifaab(bytes_read);
  err = close(pipe_desaab[INaab]);
  quitifaab(err);
	
  err = write(OUTaab, buff, bytes_read); 
  quitifaab(err);
}

void err_handleraab(int sig){
  int i;
  char arith_err[128] = "";
  char mem_err[128] = "";
  char sys_err[128] = "";
  char indent[128] = "";

  for (i = 0 ; i < INDENTaab ; i++)
    indent[i] = ' ';  
  sprintf(arith_err, "%s%s", indent, "error: arithmetic\n");
  sprintf(mem_err, "%s%s", indent, "error: memory\n");
  sprintf(sys_err, "%s%s", indent, "error: memory\n");

  switch (sig){
    case SIGFPE:
      write(pipe_desaab[OUTaab], arith_err, strlen(arith_err));
    break;

    case SIGBUS:
      write(pipe_desaab[OUTaab], mem_err, strlen(mem_err));
    break;

    case SIGSEGV:
      write(pipe_desaab[OUTaab], mem_err, strlen(mem_err));
    break;

    case SIGSYS:
      write(pipe_desaab[OUTaab], sys_err, strlen(sys_err));
    break;

    case SIGILL:
      write(pipe_desaab[OUTaab], mem_err, strlen(mem_err));
    break;
  }
  raise(sig);
}

void handle_signalsaab(){
  struct sigaction act;

  sigemptyset(&act.sa_mask);
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESETHAND;
  act.sa_handler = err_handleraab;

  sigaction(SIGFPE, &act, NULL);
  sigaction(SIGBUS, &act, NULL);
  sigaction(SIGSEGV, &act, NULL);
  sigaction(SIGSYS, &act, NULL);
  sigaction(SIGILL, &act, NULL);
  sigaction(SIGSYS, &act, NULL);
}

void run_testsaab(){
  int err;
  pid_t stat;

  /* Each of the test functions will be called in its own fork */
  void (** unit_test) (void);
  void (* test_fns[N_UNIT_TESTSaab + 1]) (void) = 
    {
      test_multiple_words,
      test_one_word,
      test_unique_words,
      test_repeat_word, 
      test_zero_words
     ,NULL
    };

  for (unit_test = test_fns ; *unit_test ; unit_test++){

    err = pipe(pipe_desaab);
    quitifaab(err);

    switch (fork()){

      case -1:
        quitifaab(-1);
        break;

      /* Execute the unit test in the child */
      case 0:
        close(pipe_desaab[INaab]);

        handle_signalsaab();
        
        (*unit_test)();
        err = close(pipe_desaab[OUTaab]);
        quitifaab(err);

        exit(0);
        break;

      /* Wait for the child to finish the unit test */
      default:
        close(pipe_desaab[OUTaab]);

        err = wait(&stat);
        quitifaab(err);

        print_resultsaab();
        break;
    }
  }
}

int main(){
  run_testsaab();
  return 0;
}
