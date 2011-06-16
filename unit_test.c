#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

/* Specifies the number of unit tests */
#define N_UNIT_TESTS 5

/* The maxium length of the YAML output string */
#define MAX_YAML_LEN 1024

/* The indentation in spaces in the yaml string */
#define INDENT 4

/* File descriptors for stdin and stdout */
#define IN 0
#define OUT 1

/*start_prototype*/
void remove_char(char c, char *str){

  /* Your C program goes here */

}
/*end_prototype*/

/* Pipe used to communicate with each forked process */
int pipe_des[2];

/* Used to ditch when something goes wrong */
void quitif(int err){
  if (-1 == err){
    perror("error");
    exit(1);
  }
}

/* Used to build a yaml string */
struct yaml_string{
  char yaml[MAX_YAML_LEN];
  int write;
};

struct yaml_string * create_yaml(){
  struct yaml_string * y;
  y = calloc(sizeof(struct yaml_string), sizeof(char));
  y->write = 0;
  return y;
}

void append_line(struct yaml_string * s, char * str, int indent){
  int i;
  int len = strlen(str);

  if (s->write + len > MAX_YAML_LEN)
    return;

  indent = indent < 0 ? 0 : indent;
  for (i = 0 ; i < indent ; i++)
    s->yaml[s->write++] = ' ';

  for (i = 0 ; i < len ; i++)
    s->yaml[s->write++] = str[i];

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

void test_first_char(){
  char str[128] = "abcd";
  struct yaml_string * ys = create_yaml();

  print_test_info(ys, "test_first_char", "a abcd", "bcd", "20");

  remove_char('a', str);
  str[sizeof(str) - 1] = '\0'; 

  print_test_output(ys, str); 
}

void test_last_char(){
  char str[128] = "abcde";
  struct yaml_string * ys = create_yaml();

  print_test_info(ys, "test_last_char", "e abcde", "abcd", "20");
  
  remove_char('e', str);
  str[ sizeof(str) - 1] = '\0';
  
  print_test_output(ys, str);
}

void test_3(){
  char str[128] = "abcde";
  struct yaml_string * ys = create_yaml();

  print_test_info(ys, "test_3", "e abcde", "abcd", "20");
  
  remove_char('e', str);
  str[ sizeof(str) - 1] = '\0';
  
  print_test_output(ys, str);
}

void test_4(){
  char str[128] = "abcde";
  struct yaml_string * ys = create_yaml();

  print_test_info(ys, "test_4", "e abcde", "abcd", "20");

  remove_char('e', str);
  str[ sizeof(str) - 1] = '\0';
  
  print_test_output(ys, str);
}

void test_5(){
  char str[128] = "abcde";
  struct yaml_string * ys = create_yaml();

  print_test_info(ys, "test_5", "e abcde", "abcd", "20");
  
  remove_char('e', str);
  str[ sizeof(str) - 1] = '\0';
  
  print_test_output(ys, str);
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
    {test_first_char,
     test_last_char,
     test_3,
     test_4,
     test_5, NULL};

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
