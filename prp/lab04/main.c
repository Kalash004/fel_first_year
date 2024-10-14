#include <stdio.h>

#define BAD_INPUT 100
#define OK 0

int read_input(int *pNum) {
  int ret_scan = scanf("%d", pNum);
  if (ret_scan != 2)
    return BAD_INPUT;
  return OK;
}

void print_line(int amount) {
  for (int i = 0; i < amount; ++i) {
    putchar('*');
  }
  putchar('\n');
}

int main(void) {
  int num = 0;
  int ret = read_input(&num);
  if (ret != OK) {
    return ret;
  }
  for (int i = 0; i < num + 1; ++i) {
    print_line(i);
  }
  return ret;
}
