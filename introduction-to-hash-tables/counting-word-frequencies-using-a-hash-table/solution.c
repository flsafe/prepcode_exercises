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


#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#define N_UNIT_TESTS 5

/* File descriptors for stdin and stdout */
#define IN 0
#define OUT 1

/* Pipe used to communicate with each forked process */
int pipe_des[2];

/* The maxium length of the YAML output string */
#define MAX_YAML_LEN 1024

/* The indentation in spaces in the yaml string */
#define INDENT 4

void print(){
  int b;

  printf("***\n");
  for (b = 0 ; b < HASH_TAB ; b++)
    if (hashtab[b].key)
      printf("%s %d\n", hashtab[b].key, hashtab[b].count);
}

void clear(){
  int b;

  for (b = 0 ; b < HASH_TAB ; b++){
    hashtab[b].key = NULL;
    hashtab[b].count = 0;
  }
}

void test_multiple_words(){
  char *input[] = {"at", "at", "at", "cat", "mat", "cat", "on", "the"};

  count_words(input, sizeof(input)/sizeof(char*));
  print();
  clear();
}

void test_one_word(){
  char *input[] = {"programming"};

  count_words(input, sizeof(input)/sizeof(char*));
  print();
  clear();
}

void test_unique_words(){
  char *input[] = {"a", "b", "c"};

  count_words(input, sizeof(input)/sizeof(char*));
  print();
  clear();
}

void test_repeat_word(){
  char *input[] = {"a", "a", "a", "a", "a"};

  count_words(input, sizeof(input)/sizeof(char*));
  print();
  clear();
}

/* Used to ditch when something goes wrong */
void quitif(int err){
  if (-1 == err){
    perror("error");
    exit(1);
  }
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
    {test_multiple_words,
     test_one_word,
     test_unique_words,
     test_repeat_word, NULL};

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
