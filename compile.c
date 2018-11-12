#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct instr {
  char val;
  struct instr* prev;
  struct instr* next;
} instr;

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

void compile(instr* cinst){
  FILE *fp = fopen("btest.asm", "w+");
  int nextLabelCount = 0;
  int bMatchCount = 0;
  int bMatchLabel[255];
  char nextLabel[20];
  char temp[500];
  fputs("bits 32\nglobal main\nsection .text\nmain:\n  mov eax, 0\n  push eax\n  push esp\n", fp);
  while(cinst->next != NULL){
    //fputc(cinst->val, fp);
    switch (cinst->val) {
      case '+':
        fputs("  pop ebx\n  mov eax, [ebx]\n  add eax, 1\n  mov [ebx], eax\n  push ebx\n", fp);
        break;
      case '-':
        fputs("  pop ebx\n  mov eax, [ebx]\n  sub eax, 1\n  mov [ebx], eax\n  push ebx\n", fp);
        //fputs("  pop ebx\n  cmp ebx, esp\n  jne  mov eax, [ebx]\n  sub eax, 1\n  mov [ebx], eax\n  push ebx\n", fp);
        break;
      case '>':
        sprintf(temp, "  pop ebx\n  cmp ebx, esp\n  jne n%d\n  mov eax, 0\n  push eax\n  n%d:\n  sub ebx, 4\n  push ebx\n", nextLabelCount, nextLabelCount);
        fputs(temp, fp);
        nextLabelCount++;
        break;
      case '<':
        fputs("  pop ebx\n  add ebx, 4\n  push ebx\n", fp);
        break;
      case '[':
        bMatchLabel[bMatchCount] = nextLabelCount;
        sprintf(temp, "  b%da0:\n  pop ebx\n  mov eax, [ebx]\n  push ebx\n  cmp eax, 0\n  je b%da1\n", bMatchLabel[bMatchCount], bMatchLabel[bMatchCount]);
        fputs(temp, fp);
        bMatchCount++;
        break;
      case ']':
        bMatchCount--;
        sprintf(temp, "  b%da1:\n  pop ebx\n  mov eax, [ebx]\n  push ebx\n  cmp eax, 0\n  jne b%da0\n", bMatchLabel[bMatchCount], bMatchLabel[bMatchCount]);
        fputs(temp, fp);
        break;
      case '.':
        fputs("  pop ecx\n  push ecx\n  push ecx\n  pop ecx\n  mov eax, 4\n  mov ebx, 1\n  mov edx, 4\n  int 0x80\n", fp);
        break;
    }
    cinst = cinst->next;
  }
  fputs("  mov eax, 1\n  mov ebx, 0\n  int 0x80\n", fp);
  fclose(fp);
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
  compile(first);
  freeinstr(first);
  printf("\n");
}
