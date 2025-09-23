// Directives to make interresting windows visible
#pragma qtrvsim show registers
#pragma qtrvsim show memory

.globl _start

.option norelax

.text

// int *pa;

_start:
main:
loop:
	la     x5, vect_a  // pa = vec_a
	addi   x8, x0, 16  // int i = 16
vect_next:                   // do {
	lw     x2, 0(x5)   // r2 = *pa
	addi   x2, x2, 1   // r2 = r2 + 1
	sw     x2, 0(x5) // *pa = r2

	addi   x5, x5, 4   // pa++
	addi   x8, x8, -1  // i--
	bne    x8, x0, vect_next  // } while (i != 0)

	// stop execution wait for debugger/user
	ebreak
	// ensure that continuation does not
	// interpret random data
	beq    x0, x0, loop

.bss

.org 0x400

.data

vect_a:	// int vect_a[16] = {1, 2, 3, ...};
	.word  0x01
	.word  0x02
	.word  0x03
	.word  0x04
	.word  0x05
	.word  0x06
	.word  0x07
	.word  0x08
	.word  0x09
	.word  0x0a
	.word  0x0b
	.word  0x0c
	.word  0x0d
	.word  0x0e
	.word  0x0f
	.word  0x00

// Specify location to show in memory window
#pragma qtrvsim focus memory vect_a

