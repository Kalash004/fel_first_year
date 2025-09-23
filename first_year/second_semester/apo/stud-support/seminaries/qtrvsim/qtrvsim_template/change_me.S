/* template for own QtRvSim program development */

.globl _start
.globl __start

.option norelax

.text

__start:
_start:
	addi    a0, zero, 10
	la	a1, var1
	lw	a2, 0(a1)

	/* space for your code */

loop:	ebreak
        beq     zero, zero, loop

.data

var1:	.word 0x12345678
