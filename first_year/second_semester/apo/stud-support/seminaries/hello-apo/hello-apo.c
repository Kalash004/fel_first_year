/* Hello world example to demonstrate on multiple architectures */

/*
 * compile, run and examine for different instruction set architectures
 * build by make command where ARCH is chisen as one of
 *   native, x86, riscv, riscv64, mips, arm or aarch64
 *   make ARCH=riscv
 */

#include <stdio.h>

int var_a = 40;
int var_b =  2;

int main(int argc, char *argv[])
{

  /*
   * modify program to compute and print answer
   * of var_a + var_b
   */
  int result = var_a + var_b;
  printf("Hello APO %d\n", result);

  return 0;
}
