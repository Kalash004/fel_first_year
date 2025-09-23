.globl  _start

#pragma qtrvsim show registers
#pragma qtrvsim show memory
#pragma qtrvsim show peripherals

.equ SPILED_REG_LED_LINE,   0xffffc104 // 32 bit word mapped as output
.equ STACK_INIT, 0x01230000 // The bottom of the stack, the stack grows down

.option norelax
.text

_start:
main:
	li     sp, STACK_INIT
	addi   a0, zero, 4	// 4! -> 24
	jal    fact
	addi   t0, a0, 0

	li     t1, SPILED_REG_LED_LINE
	sw     t0, 0(t1)
final:
	ebreak
	beq    zero, zero, final
	nop

fact:
	addi   sp, sp, -4   // adjust stack for 1 item
	sw     a0, 0(sp)    // save argument
	slti   t0, a0, 1    // test for n < 1
	beq    t0, zero, L1
	addi   a0, zero, 1  // if so, result is 1
	addi   sp, sp, 4    // pop 1 item from stack
	ret                 // and return (jr ra)
L1:	addi   a0, a0, -1   // else decrement n
	jal    fact         // recursive call
	lw     t0, 0(sp)    // restore original n
	addi   sp, sp, 4    // pop 2 items from stack
	mul    a0, a0, t0   // multiply to get result
	jr     ra           // and return

#pragma qtrvsim focus memory STACK_INIT-4*8
