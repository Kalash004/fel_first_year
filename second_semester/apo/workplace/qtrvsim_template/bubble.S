/* template for own QtRvSim program development */

.globl _start
.globl __start

.option norelax

.text

__start:
_start:
	la x10,arr /*pole*/
	addi x8,x0,0 /*i*/

forout:
	addi x9,x0,0 /*j*/
	addi x1,x0,16 /* N-1 */
	sub x1,x1,x8 /*N-1-i*/


forin:
	add x11,x10,x9
	lw x4,0(x11) /*pole[j]*/
	lw x5,4(x11) /*pole[j+1]*/
	slt x6,x5,x4 /** pole[j+1] < pole[j] */
	beq x6,x0,skip

	sw x5,0(x11)
	sw x4,4(x11)

skip:
	addi x9,x9,4 /*j++*/
	bne x9,x1,forin

	addi x8,x8,4
	addi x1,x0,16
	bne x8,x1,forout 

loop:	ebreak
        beq zero, zero, loop

.data

arr: .word 5, 3, 4, 1, 2