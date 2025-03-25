/* template for own QtRvSim program development */

.globl _start
.globl __start

.option norelax

.text

__start:
_start:
	la x1, arr
	lw x2, 0(x1)
	lw x3, 4(x1)
	add x4,x3,x2
	sw x4,0(x1)

loop: ebreak
		beq zero, zero, loop; # if zero == zero then loop
			

.data

var1:	.word 0x12345678
arr: .word 1, 2, 3, 4, 5