#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#define N_UNIT_TESTS 2
#define MAX_YAML_LEN 1024
#define INDENT 4
#define IN 0
#define OUT 1

void remove_char(char c, char *str){}

/* Used to ditch when something goes wrong */
void quitif(int err){
  if (-1 == err){
    perror("error");
    exit(1);
  }
}

/* Pipe used to communicate with each forked process */
int pipe_des[2];

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

  /* TODO: Add error checking. Make sure we don't write past 
   * the yaml buffer */

  int len = strlen(str);
  int i;

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

void print_test_output(struct yaml_string * ys, char *str){
  append_line(ys, "output: |", INDENT);
    append_line(ys, str, INDENT * 2);
  flush_yaml(ys);
}

void test_first_char(){

  /* TODO: Write the test case */
  struct yaml_string * ys = create_yaml();

  print_test_info(ys, "test_first_char", "a abcd", "bcd", "10");
}

void test_last_char(){

  /* TODO: Write the test case */
  struct yaml_string * ys = create_yaml();

  print_test_info(ys, "test_last_char", "a abcd", "bcd", "10");
}

void collect_results(){

  /* TODO: Collect the yaml strings from each unit test */
  int bytes_read, err;
  char buff[MAX_YAML_LEN] = "";

  bytes_read = read(pipe_des[IN], buff, MAX_YAML_LEN);
  quitif(bytes_read);
  err = close(pipe_des[IN]);
  quitif(err);
	
  err = write(OUT, buff, bytes_read); 
  quitif(err);
}

void run_tests(){
  int err;
  pid_t stat;

  /* Each of the test functions will be called in its own fork */
  void (** unit_test) (void);
  void (* test_fns[N_UNIT_TESTS + 1]) (void) = 
    {test_first_char,
     test_last_char, NULL};

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

        collect_results();
        break;
    }
  }
}

int main(){
  run_tests();
  return 0;
}
