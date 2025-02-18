// select core without pipeline but with delay slot

#pragma qtmips show registers
#pragma qtmips show memory

.equ SPILED_REG_LED_LINE,   0xffffc104 // 32 bit word mapped as output
.equ STACK_INIT, 0x01230000 // The bottom of the stack, the stack grows down

.set noreorder
.text

.globl start
.globl _start

// int main(){
//  int res;
//  res = complex(1,2,3,4,5,6);
//  return res;
// }

// mips-elf-gcc -c -O6 lec10-call-6args.c
// mips-elf-objdump --source -M no-aliases   lec10-call-6args.o

start:
_start:
	la   $sp, STACK_INIT
	jal  main
	nop
	sw   $v0, SPILED_REG_LED_LINE($0)
final:
	break
	beq  $0, $0, final
	nop

main:
	addiu   $sp, $sp, -32	// allocate space for save and arguments
	sw      $ra, 28($sp)	// store return address from main

	sw      $s8, 24($sp)	// store farme pointer, not required there
	or      $s8, $sp, $zero	// set fp to point to top of the stack, not required
				// see -fomit-frame-pointer/-fno-omit-frame-pointer

	addiu   $v0, $zero, 6	// 6-th argument value
	sw      $v0, 20($sp)	// 6-th argument to the stack
	addiu   $v0, $zero, 5	// 5-th argument value
	sw      $v0, 16($sp)	// 5-th argument to the stack
	addiu   $a3, $zero, 4	// 4-th argument value
	addiu   $a2, $zero, 3	// 3-rd argument value
	addiu   $a1, $zero, 2	// 2-nd argument value
	jal     complex		// call complex function
	addiu   $a0, $zero, 1	// store 1-st argument, run in delay slot

	or      $sp, $s8, $zero	// restore stack from frame pointer
	lw      $s8, 24($sp)	// restore frame pointer, not required there

	lw      $ra, 28($sp)	// restore return address
	nop			// on MIPS1 lw result not available directly
	jr      $ra		// return from main, result in v0
	addiu   $sp, $sp, 32	// free stack frame, run in delay slot


complex:
	// sequnece to allocate function frame
	addiu   $sp, $sp, -24	// allocate frame for the function
	sw      $s8, 16($sp)	// store previous fp value
	sw      $ra, 20($sp)	// store return address to allow call subroutine,
				// it is not required in leaf node function
	or      $s8, $sp, $zero	// set fp to point to top of the stack

	// function body, stack can be freely allocated
	// to pas arguments and store local variables
	addu    $a0, $a0, $a1	// add the arg 1-st and 2-nd
	lw      $v0, 40($s8)	// load 5-th argument 24 + 16
	addu    $a2, $a0, $a2	// add the 3-rd one to the sum
	addu    $a3, $a2, $a3	// add the 4-th one to the sum
	lw      $t0, 44($s8)	// load 6-th argument 24 + 20
	addu    $v0, $a3, $v0	// add 5-th argument
	addu    $v0, $v0, $t0	// add 6-th one to the summ

	// sequence to leave function frame
	or      $sp, $s8, $zero	// restore stack from frame pointer
	lw      $ra, 20($sp)	// restore return address, not necessary in leaf node
	lw      $s8, 16($sp)	// restore previous frame pointer
	jr      $ra		// return from subroutine
	addiu   $sp, $sp, 24	// free stack frame in delay slot

#pragma qtmips focus memory STACK_INIT-32-24
