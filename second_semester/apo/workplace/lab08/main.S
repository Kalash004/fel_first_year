#define SPILED_REG_BASE    0xffffc100

#define SPILED_REG_LED_LINE_o           0x004
#define SPILED_REG_LED_RGB1_o           0x010
#define SPILED_REG_LED_RGB2_o           0x014
#define SPILED_REG_LED_KBDWR_DIRECT_o   0x018

#define SPILED_REG_KBDRD_KNOBS_DIRECT_o 0x020
#define SPILED_REG_KNOBS_8BIT_o         0x024

.globl _start
.globl __start

.option norelax
.text
__start:
_start:
la a0, SPILED_REG_BASE
addi a3, zero, 0x42;
sw a3, SPILED_REG_BASE(a0)  

