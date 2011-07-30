/*start_prototype*/
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
/*end_prototype*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* The maxium length of the YAML output string */
#define MAX_YAML_LEN 10000 

/* The indentation in spaces in the yaml string */
#define INDENT 4

/* Max output of the function under test */
#define MAX_OUT_BUFF 10000 

struct yaml_string{
  char yaml[MAX_YAML_LEN];
  int write;
};
struct yaml_string * create_yaml(void);
void print_test_info(struct yaml_string * ys,
                     char * name,
                     char * input, 
                     char * expected, 
                     char * points);
void print_test_output(struct yaml_string * ys, char * str);

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
  struct yaml_string * ys = create_yaml();

  print_test_info(ys, "test_multiple_words", 
                      "the cat the hat the mat", 
                      "cat 1\nhat 1\nmat 1\nthe 3", 
                      "20");

  clear_hashtab();
  count_words(words, sizeof(words)/sizeof(char*));

  qsort(hashtab, HASH_TAB, sizeof(struct hashrec), cmp);
  print_hashtab(buff, MAX_OUT_BUFF);
  print_test_output(ys, buff); 
}

void test_one_word(void){
  char buff[MAX_OUT_BUFF] = "";
  char *words[] = {"programming"};
  struct yaml_string * ys = create_yaml();

  print_test_info(ys, "test_one_word", "programming", "programming 1", "20");
  
  clear_hashtab();
  count_words(words,sizeof(words)/sizeof(char*));

  qsort(hashtab, HASH_TAB, sizeof(struct hashrec), cmp);
  print_hashtab(buff, MAX_OUT_BUFF);
  print_test_output(ys, buff); 
}

void test_unique_words(void){
  char buff[MAX_OUT_BUFF] = "";
  char *words[] = {"cat", "sat", "on", "the", "mat"};
  struct yaml_string * ys = create_yaml();

  print_test_info(ys, "test_unique_words", 
                      "cat sat on the mat", 
                      "cat 1\nmat 1\non 1\nsat 1\nthe 1", 
                      "20");

  clear_hashtab();
  count_words(words, sizeof(words)/sizeof(char*));

  qsort(hashtab, HASH_TAB, sizeof(struct hashrec), cmp);
  print_hashtab(buff, MAX_OUT_BUFF);
  print_test_output(ys, buff); 
}

void test_repeat_word(void){
  char buff[MAX_OUT_BUFF] = "";
  char *words[] = {"a", "a", "a", "a", "a"};
  struct yaml_string * ys = create_yaml();

  print_test_info(ys, "test_repeat_word", "a a a a a", "a 5", "20");

  clear_hashtab();
  count_words(words, sizeof(words)/sizeof(char*));

  qsort(hashtab, HASH_TAB, sizeof(struct hashrec), cmp);
  print_hashtab(buff, MAX_OUT_BUFF);
  print_test_output(ys, buff); 
}

void test_zero_words(void){
  char buff[MAX_OUT_BUFF] = "";
  char *words[] = {NULL};
  struct yaml_string * ys = create_yaml();

  print_test_info(ys, "test_zero_words", "", "", "20");

  clear_hashtab();
  count_words(words, 0);

  qsort(hashtab, HASH_TAB, sizeof(struct hashrec), cmp);
  print_hashtab(buff, MAX_OUT_BUFF);
  print_test_output(ys, buff); 
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
#define N_UNIT_TESTS 5

/* File descriptors for stdin and stdout */
#define IN 0
#define OUT 1

/* Pipe used to communicate with each forked process */
int pipe_des[2];

/* Used to ditch when something goes wrong */
void quitif(int err){
  if (-1 == err){
    perror("error");
    exit(1);
  }
}

struct yaml_string * create_yaml(){
  struct yaml_string * y;
  y = calloc(sizeof(struct yaml_string), sizeof(char));
  y->write = 0;
  return y;
}

void append_line(struct yaml_string * s, char * str, int indent){
  int i,j;
  int len = strlen(str);

  if (s->write + len > MAX_YAML_LEN)
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

void flush_yaml(struct yaml_string * ys){
  int i, bytes_written;

  bytes_written = write(pipe_des[OUT], ys->yaml, ys->write);
  quitif(bytes_written);

  for (i = ys->write - 1 ; i ; i--)
    ys->yaml[i] = '\0';
  ys->write = 0;
}

void print_test_info(struct yaml_string * ys,
    char * name,
    char * input, 
    char * expected, 
    char * points){

  char test_name[256];

  sprintf(test_name, "%s:", name);
  append_line(ys, test_name, 0);
    append_line(ys, "input: |", INDENT);
      append_line(ys, input, INDENT * 2);

    append_line(ys, "expected: |", INDENT);
      append_line(ys, expected, INDENT * 2);

    append_line(ys, "points: |", INDENT);
      append_line(ys, points, INDENT * 2);

  flush_yaml(ys);
}

void print_test_output(struct yaml_string * ys, char * str){
  append_line(ys, "output: |", INDENT);
    append_line(ys, str, INDENT * 2);
  flush_yaml(ys);
}

void print_results(){
  int bytes_read, err;
  char buff[MAX_YAML_LEN] = "";

  bytes_read = read(pipe_des[IN], buff, MAX_YAML_LEN);
  quitif(bytes_read);
  err = close(pipe_des[IN]);
  quitif(err);
	
  err = write(OUT, buff, bytes_read); 
  quitif(err);
}

void err_handler(int sig){
  int i;
  char arith_err[128] = "";
  char mem_err[128] = "";
  char sys_err[128] = "";
  char indent[128] = "";

  for (i = 0 ; i < INDENT ; i++)
    indent[i] = ' ';  
  sprintf(arith_err, "%s%s", indent, "error: arithmetic\n");
  sprintf(mem_err, "%s%s", indent, "error: memory\n");
  sprintf(sys_err, "%s%s", indent, "error: memory\n");

  switch (sig){
    case SIGFPE:
      write(pipe_des[OUT], arith_err, strlen(arith_err));
    break;

    case SIGBUS:
      write(pipe_des[OUT], mem_err, strlen(mem_err));
    break;

    case SIGSEGV:
      write(pipe_des[OUT], mem_err, strlen(mem_err));
    break;

    case SIGSYS:
      write(pipe_des[OUT], sys_err, strlen(sys_err));
    break;

    case SIGILL:
      write(pipe_des[OUT], mem_err, strlen(mem_err));
    break;
  }
  raise(sig);
}

void handle_signals(){
  struct sigaction act;

  sigemptyset(&act.sa_mask);
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESETHAND;
  act.sa_handler = err_handler;

  sigaction(SIGFPE, &act, NULL);
  sigaction(SIGBUS, &act, NULL);
  sigaction(SIGSEGV, &act, NULL);
  sigaction(SIGSYS, &act, NULL);
  sigaction(SIGILL, &act, NULL);
  sigaction(SIGSYS, &act, NULL);
}

void run_tests(){
  int err;
  pid_t stat;

  /* Each of the test functions will be called in its own fork */
  void (** unit_test) (void);
  void (* test_fns[N_UNIT_TESTS + 1]) (void) = 
    {
     test_multiple_words,
     test_one_word,
     test_unique_words,
     test_repeat_word,
     test_zero_words
     ,NULL
    };

  for (unit_test = test_fns ; *unit_test ; unit_test++){

    err = pipe(pipe_des);
    quitif(err);

    switch (fork()){

      case -1:
        quitif(-1);
        break;

      /* Execute the unit test in the child */
      case 0:
        close(pipe_des[IN]);

        handle_signals();
        
        (*unit_test)();
        err = close(pipe_des[OUT]);
        quitif(err);

        exit(0);
        break;

      /* Wait for the child to finish the unit test */
      default:
        close(pipe_des[OUT]);

        err = wait(&stat);
        quitif(err);

        print_results();
        break;
    }
  }
}

int main(){
  run_tests();
  return 0;
}
