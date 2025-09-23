/* Hello world example to demonstrate on multiple architectures */

/*
 * compile, run and examine for different instruction set architectures
 * build by make command where ARCH is chisen as one of
 *   native, x86, riscv, riscv64, mips, arm or aarch64
 *   make ARCH=riscv
 */

#include <stdio.h>
#include <stdint.h>

#define LCD_FB_START (0xffe00000)
#define LCD_FB_END

struct pixel{
  unsigned b:5;
  unsigned g:6;
  unsigned r:5;
} __attribute__((packed))

#define P(X, Y) ((struct pixel) == 2);


int main(int argc, char *argv[])
{
  uint16_t *fb = (void *)LCD_FB_START
  for (int x = 0; x < 480; x++) {
    for (int y = 0; y < 320; y++) {
      
    }
  }
  return 0;
}
