#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct instr {
  char val;
  struct instr* prev;
  struct instr* next;
} instr;

typedef struct data {
  int val;
  struct data* prev;
  struct data* next;
} data;

void freedata(data* cdata){
  while(cdata->next != NULL){
    cdata = cdata->next;
  }
  while(cdata->prev != cdata){
    cdata = cdata->prev;
    free(cdata->next);
  }
  free(cdata);
}

void freeinstr(instr* cinstr){
  while(cinstr->next != NULL){
    cinstr = cinstr->next;
  }
  while(cinstr->prev != cinstr){
    cinstr = cinstr->prev;
    free(cinstr->next);
  }
  free(cinstr);
}

void interpret(instr* cinst){
  data* first = calloc(1, sizeof(data));
  data* current = first;
  current->val = 0;
  current->prev = current;
  while(cinst->next != NULL){
    switch (cinst->val) {
      case '+':
        current->val++;
        break;
      case '-':
        current->val--;
        break;
      case '>':
        if(current->next != NULL){
          current = current->next;
        }else {
          current->next = calloc(1, sizeof(data));
          current->next->prev = current;
          current = current->next;
          current->val = 0;
        }
        break;
      case '<':
        current = current->prev;
        break;
      case '[':
        if(current->val == 0){
          int tomatch = 1;
          while(tomatch > 0){
            cinst = cinst->next;
            if(cinst->val == '['){
              tomatch++;
            }else if(cinst->val == ']'){
              tomatch--;
            }
          }
        }
        break;
      case ']':
        if(current->val != 0){
          int tomatch = 1;
          while(tomatch > 0){
            cinst = cinst->prev;
            if(cinst->val == ']'){
              tomatch++;
            }else if(cinst->val == '['){
              tomatch--;
            }
          }
        }
        break;
      case '.':
        printf("%c", (char)(current->val));
        break;
    }
    cinst = cinst->next;
  }
  freedata(first);
}

int main(int argc, char *argv[]){
  char c;
  int total = 0;
  FILE *fp = fopen(argv[1], "r");
  instr* first;
  instr* current;
  first = malloc(sizeof(instr));
  current = first;
  current->prev = current;
  while(feof(fp) == 0) {
    c = fgetc(fp);
    if(c == '+' || c == '-' || c == '>' || c == '<'
      || c == ',' || c == '.' || c == '[' || c == ']'){
      current->val = c;
      current->next = malloc(sizeof(instr));
      current->next->prev = current;
      current = current->next;
      total++;
    }
  }
  fclose(fp);
  printf("\n");
  interpret(first);
  freeinstr(first);
  printf("\n");
}
