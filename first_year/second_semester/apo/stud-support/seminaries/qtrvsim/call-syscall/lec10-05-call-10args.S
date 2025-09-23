.globl  _start

#pragma qtrvsim show registers
#pragma qtrvsim show memory

.equ SPILED_REG_LED_LINE,   0xffffc104 // 32 bit word mapped as output
.equ STACK_INIT, 0x01230000 // The bottom of the stack, the stack grows down

.option norelax
.text

.globl start
.globl _start

// int main(){
//  int res;
//  res = complex(1,2,3,4,5,6,7,8,9,10);
//  return res;
// }

// riscv64-unknown-elf-gcc -D__ASSEMBLY__ -ggdb -mabi=ilp32 -march=rv32i -fno-lto -c lec10-call-10args.S -o lec10-call-10args.o
// riscv64-unknown-elf-gcc -nostartfiles -nostdlib -static -mabi=ilp32 -march=rv32i lec10-call-10args.o -o lec10-call-10args


start:
_start:
	la      sp, STACK_INIT
	jal     main
	li      t0, SPILED_REG_LED_LINE
	sw      a0, 0(t0)
final:
	ebreak
	beq     zero, zero, final
	nop

main:
	addi    sp, sp, -48	// allocate space for save and arguments
	sw      ra, 44(sp)	// store return address from main

	sw      s0, 40(sp)	// store farme pointer, not required there
	addi    s0, sp, 48	// set fp to point to top of the stack, not required
				// see -fomit-frame-pointer/-fno-omit-frame-pointer

	addi    t0, zero, 10	// 10-th argument value
	sw      t0, 4(sp)	// 10-th argument to the stack
	addi    t0, zero, 9	// 9-th argument value
	sw      t0, 0(sp)	// 9-th argument to the stack
	addi    a7, zero, 8	// 8-th argument value
	addi    a6, zero, 7	// 7-rd argument value
	addi    a5, zero, 6	// 6-nd argument value
	addi    a4, zero, 5	// 5-st argument value
	addi    a3, zero, 4	// 4-th argument value
	addi    a2, zero, 3	// 3-rd argument value
	addi    a1, zero, 2	// 2-nd argument value
	addi    a0, zero, 1	// store 1-st argument value
	jal     complex		// call complex function

	sw      a0, -20(s0)	// store return value into local variable
	lw      a5, -20(s0)	// move back to a0, caused by no optmalization
	mv      a0, a5

	addi    sp, s0, -48	// restore stack from frame pointer
	lw      ra, 44(sp)	// restore return address
	lw      s0, 40(sp)	// restore frame pointer, not required there
	addi    sp, sp, 48	// free stack frame
	jr      ra		// return from main, result in a0


complex:
	// sequnece to allocate function frame
	addi    sp, sp, -48	// allocate frame for the function
	sw      s0, 40(sp)	// store previous fp value
	sw      ra, 44(sp)	// store return address to allow call subroutine,
				// it is not required in leaf node function
	addi    s0, sp, 48	// set fp to point to top of the stack

	// function body, stack can be freely allocated
	// to pas arguments and store local variables
	add     a0, a0, a1	// add the arg 1-st and 2-nd
	add     a0, a0, a2	// add the arg 3-rd
	add     a0, a0, a3	// add the arg 4-th
	add     a0, a0, a4	// add the arg 5-th
	add     a0, a0, a5	// add the arg 6-th
	add     a0, a0, a6	// add the arg 7-th
	add     a0, a0, a7	// add the arg 8-th

	lw      a5, 0(s0)	// load 9-th argument
	add     a0, a0, a5	// add 9-th argument
	lw      a5, 4(s0)	// load 10-th argument
	add     a0, a0, a5	// add 10-th argument

	// sequence to leave function frame
	addi    sp, s0, -48	// restore stack from frame pointer
	lw      ra, 44(sp)	// restore return address, not necessary in leaf node
	lw      s0, 40(sp)	// restore previous frame pointer
	addi    sp, sp, 48	// free stack frame in delay slot
	jr      ra		// return from subroutine

#pragma qtrvsim focus memory STACK_INIT-32-24
