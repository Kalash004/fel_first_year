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
	add     t0, a0, a1
	add     t1, a2, a3
	// Clobbers caller guaranteed
	// to be saved register s0
	sub     s0, t0, t1
	add     a0, s0, zero
	ret     // jr ra

#pragma qtrvsim focus memory STACK_INIT-4*8
