#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int get_line(FILE *f, char *s, int lim);
int get_array(char *line, int **arr);
char *get_int(char *s, int *result);

int initbv(int val);
void freebv();
void set(int);
int member(int);

#define MAX_LINE 1000

int main(int argc, char **argv){
	int len,i;
	FILE *f;
	char line[MAX_LINE];
	int *arr = NULL;

	f = fopen(argv[1], "r");

	while (get_line(f, line, MAX_LINE) > 0){
		len = get_array(line, &arr);
		initbv(len);
		for (i = 0 ; i < len ; i++)
			if (!member(arr[i]))
				set(arr[i]);
			else {
				printf("%d\n", arr[i]);
				break;
			}
		free(arr);
		freebv();
	}

	return 0;
}

int get_line(FILE *f, char *s, int lim){
	int c,i;

	i = 0;
	while (--lim > 0 && (c=fgetc(f)) != EOF && c != '\n')
		s[i++] = c;
	if (c == '\n')
		s[i++] = c;
	s[i] = '\0';
	return i;
}

int get_array(char *line, int **arr){
	int len, i;
	int *p;

	if ((line = get_int(line, &len)) == NULL)
		return -1;
	if ((*arr = malloc(len * sizeof(int))) == NULL)
		return -1;

	p = *arr;
	while ((line = get_int(line, &i)))
		*p++ = i;	

	return len;
}

#define MAX_DIG 7

char *get_int(char *s, int *result){
	char sint[MAX_DIG + 1];	
	int i;

	for (i = 0 ; i < MAX_DIG && isdigit(s[i]) ; i++)
		sint[i] = s[i];
	sint[i] = '\0';

	if (sint[0]){
		*result = atoi(sint);
		return s + i + 1;
	} else {
		*result = -1;
		return NULL;
	}
}

#define WORDSIZE 32
#define BFWS 5
#define MASK 0x1F

int *bv = NULL;

int initbv(int val){
	bv = calloc(val/WORDSIZE + 1, sizeof(int));
	return bv != NULL;
}

void set(int i){
	bv[i>>BFWS] |= (1 << (i & MASK));
}

int member(int i){
	return bv[i>>BFWS] & (1 << (i & MASK));	
}

void freebv(){
	free(bv);
}
