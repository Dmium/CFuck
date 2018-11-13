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

void compile(instr* cinst, char* outfile){
  FILE *fp = fopen(outfile, "w+");
  int nextLabelCount = 0;
  int bMatchCount = 0;
  int bMatchLabel[255];
  char nextLabel[20];
  char temp[500];
  int incCounter = 0;
  int movCounter = 0;
  fputs("bits 32\nglobal main\nsection .text\nmain:\n  push 0\n  push esp\n", fp);
  while(cinst->next != NULL){
    //fputc(cinst->val, fp);
    switch (cinst->val) {
      case '+':
        incCounter++;
        break;
      case '-':
        incCounter--;
        break;
      default:
        if (incCounter > 0){
          sprintf(temp, "  mov ebx, [esp]\n  mov eax, [ebx]\n  add eax, %d\n  mov [ebx], eax\n", incCounter);
          fputs(temp, fp);
          incCounter = 0;
        }else if (incCounter < 0){
          sprintf(temp, "  mov ebx, [esp]\n  mov eax, [ebx]\n  sub eax, %d\n  mov [ebx], eax\n", 0 - incCounter);
          fputs(temp, fp);
          incCounter = 0;
        }
        break;
      }
    switch (cinst->val) {
      case '>':
        movCounter++;
        break;
      case '<':
        movCounter--;
        break;
      default:
        if (movCounter > 0){
          while (movCounter > 0){
            sprintf(temp, "  pop ebx\n  cmp ebx, esp\n  jne n%d\n  mov eax, 0\n  push eax\n  n%d:\n  sub ebx, 4\n  push ebx\n", nextLabelCount, nextLabelCount);
            fputs(temp, fp);
            nextLabelCount++;
            movCounter--;
          }
          movCounter = 0;
        }else if (movCounter < 0){
          sprintf(temp, "  pop ebx\n  add ebx, %d\n  push ebx\n", (0 - movCounter) * 4);
          fputs(temp, fp);
          movCounter = 0;
        }
        break;
      }

    switch (cinst->val) {
      case '[':
        bMatchLabel[bMatchCount] = nextLabelCount;
        sprintf(temp, "  b%da0:\n  mov ebx, [esp]\n  mov eax, [ebx]\n  cmp eax, 0\n  je b%da1\n", bMatchLabel[bMatchCount], bMatchLabel[bMatchCount]);
        fputs(temp, fp);
        bMatchCount++;
        nextLabelCount++;
        break;
      case ']':
        bMatchCount--;
        sprintf(temp, "  b%da1:\n  mov ebx, [esp]\n  mov eax, [ebx]\n  cmp eax, 0\n  jne b%da0\n", bMatchLabel[bMatchCount], bMatchLabel[bMatchCount]);
        fputs(temp, fp);
        break;
      case '.':
        fputs("  mov eax, 4\n  mov ebx, 1\n  mov ecx, [esp]\n  mov edx, 1\n  int 0x80\n", fp);
        break;
      case ',':
        sprintf(temp, "  i%d:\n  mov eax, 3\n  mov ebx, 0\n  mov ecx, [esp]\n  mov edx, 1\n  int 0x80\n  mov eax, 10\n  mov ecx, [esp]\n  mov ebx, [ecx]\n  cmp eax, ebx\n  je i%d\n", nextLabelCount, nextLabelCount);
        fputs(temp, fp);
        nextLabelCount++;
        break;
      default:
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
  printf("Parsing Input File\n");
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
      current->next = calloc(1, sizeof(instr));
      current->next->prev = current;
      current = current->next;
      total++;
    }
  }
  fclose(fp);
  current->val = 'x';
  current->next = calloc(1, sizeof(instr));
  current->next->prev = current;
  current = current->next;
  total++;
  printf("Compiling\n");
  compile(first, argv[2]);
  freeinstr(first);
  printf("Complete\n");
}
