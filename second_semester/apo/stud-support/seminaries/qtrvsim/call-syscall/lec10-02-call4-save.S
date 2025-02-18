.globl  _start
.option norelax

#pragma qtrvsim show registers
#pragma qtrvsim show memory
#pragma qtrvsim show peripherals

.equ SPILED_REG_LED_LINE,   0xffffc104 // 32 bit word mapped as output
.equ STACK_INIT, 0x01230000 // The bottom of the stack, the stack grows down

_start:

main:
	li      sp, STACK_INIT
	li      s0, 0x12345678

	addi    a0, zero, 2
	addi    a1, zero, 3
	addi    a2, zero, 4
	addi    a3, zero, 5
	jal     fun
	add     s0, a0, zero

	li      t0, SPILED_REG_LED_LINE
	sw      s0, 0(t0)
final:
	ebreak
	beq     zero, zero, final
	nop

// int fun(int g, h, i, j)
// g→a0, h→a1, i→a2, j→a3, a0 – ret. val, sX – save, tX – temp, ra – ret. addr
// return (g + h) – (i + j)
fun:
	addi    sp, sp, -4    // allocate space on stack
	sw      s0, 0(sp)     // save s0 on stack
	add     t0, a0, a1
	add     t0, a0, a1
	add     t1, a2, a3
	sub     s0, t0, t1    // s0 is used
	add     a0, s0, zero
	lw      s0, 0(sp)     // s0 is restored
	addi    sp, sp, 4     // space on stack is release
	ret     // jr ra

#pragma qtrvsim focus memory STACK_INIT-4*8
