#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK 64
#define MAX_LINE 1000

int stack[MAX_STACK];
int *stack_pointer = stack;
int *max_stack = stack + MAX_STACK;

void die(char *message)
{
  fprintf(stderr, "FATAL: %s\n", message);
  exit(EXIT_FAILURE);
}

/* Stack primitives */

int pop()
{
  if (stack_pointer==stack) die("Stack is empty");
  return *--stack_pointer;
}

void push(int number)
{
  if (stack_pointer==max_stack) die("Stack overflow");
  *stack_pointer = number;
  stack_pointer++;
}

void dup() { push(*(stack_pointer-1)); }
void swap() { int tos = pop(); dup(); *(stack_pointer-2) = tos; }

void plus() { push(pop()+pop()); }
void minus() { int tos = pop(); push(pop()-tos); }
void multiply() { push(pop()*pop()); }
void divmod() { int tos = pop(); div_t result = div(pop(),tos); push(result.quot); push(result.rem); }
void print_tos() { fprintf(stdout, "%d\n", pop()); }

void print_stack()
{
  fprintf(stdout, "[ ");
  int *i;
  for (i = stack; i < stack_pointer; i++) {
    fprintf(stdout, "%d ", *i);
  }
  fprintf(stdout, "]\n");
}

void default_handler(char *token_pointer) { push(atoi(token_pointer)); }

/* Main */

int main(int argc, const char *argv[])
{
  char line[MAX_LINE];
  char separators[] = "\n\r\t ";
  char *token_pointer;

  /*
  input = fopen(argv[1], "r");
  if (input==NULL) {
    fprintf(stderr, "Cannot open file : %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }*/

  while (fgets(line,sizeof(line)-1,stdin) != NULL) {
    /* token_pointer = line; */
    token_pointer = strtok(line, separators);
    while (token_pointer!=NULL) {
      if (strcmp(token_pointer,"+")==0) { plus(); }
      else if (strcmp(token_pointer,"-")==0) { minus(); }
      else if (strcmp(token_pointer,"*")==0) { multiply(); }
      else if (strcmp(token_pointer,"/%")==0) { divmod(); }
      else if (strcmp(token_pointer,"/")==0) { divmod(); pop(); }
      else if (strcmp(token_pointer,"%")==0) { divmod(); swap(); pop(); }
      else if (strcmp(token_pointer,"drop")==0) { pop(); }
      else if (strcmp(token_pointer,"dup")==0) { dup(); }
      else if (strcmp(token_pointer,"swap")==0) { swap(); }
      else if (strcmp(token_pointer,"=")==0) { print_tos(); }
      else if (strcmp(token_pointer,"print-stack")==0) { print_stack(); }
      else { default_handler(token_pointer); }
      token_pointer = strtok(NULL, separators);
    }
  }

  return 0;
}
