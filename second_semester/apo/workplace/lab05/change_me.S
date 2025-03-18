/* template for own QtRvSim program development */

.globl _start
.globl __start

.option norelax

.text

__start:
_start:
	
	/* space for your code */
	addi x5,x0,5 
	la x1,ar1
	la x2,ar2
	la x3,res
	
	/*
	
	*/
for: 
	lw x4,0(x1)
	lw x6,0(x2)
	add x4,x4,x6
	sw x5,0(x3)

	addi x1,x1,4
	addi x2,x2,4
	addi x3,x3,4
	addi x5,x5,-1
	bne x5,x0,for




loop:	ebreak
        beq     zero, zero, loop

.data

var1:	.word 0x12345678
ar1: .word 1, 2, 3, 4, 5
ar2: .word 6, 7, 8, 9, 10
res: