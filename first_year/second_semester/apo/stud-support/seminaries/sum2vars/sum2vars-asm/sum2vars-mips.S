.globl main
.globl _start

.text
_start:
main:
	lw   $4, var_a($0)
	lw   $5, var_b($0)
	add  $6, $4, $5
	sw   $6, var_c($0)
	
	addi $2, $0, 0
	jr   $ra
	nop

.data

var_a:	.word 0x1234;
var_b:	.word 0x2222;
var_c:	.word 0x3333;

#pragma qtmips show registers
#pragma qtmips show memory
#pragma qtmips focus memory var_a
#pragma qtmips tab core
