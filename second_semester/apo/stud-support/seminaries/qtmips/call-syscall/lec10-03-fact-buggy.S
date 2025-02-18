// select core without pipeline but with delay slot

#pragma qtmips show registers
#pragma qtmips show memory
#pragma qtmips show peripherals

.equ SPILED_REG_LED_LINE,   0xffffc104 // 32 bit word mapped as output
.equ STACK_INIT, 0x01230000 // The bottom of the stack, the stack grows down

.set noreorder
.text

main:
	la   $sp, STACK_INIT
	addi $a0, $0, 4	// 4! -> 24
	jal  fact
	nop
	add  $s0, $v0, $0
	sw   $s0, SPILED_REG_LED_LINE($0)
final:
	break
	beq  $0, $0, final
	nop

fact:
	addi  $sp, $sp, -4   // adjust stack for 2 items
	sw    $a0, 0($sp)    // save argument
	slti  $t0, $a0, 1    // test for n < 1
	beq   $t0, $zero, L1
	addi  $v0, $zero, 1  // if so, result is 1
	addi  $sp, $sp, 4    // pop 2 items from stack
	jr    $ra            // and return
	nop
L1:	addi $a0, $a0, -1    // else decrement n
	jal   fact           // recursive call
	nop
	lw    $a0, 0($sp)    // restore original n
	addi  $sp, $sp, 4    // pop 2 items from stack
	mul   $v0, $a0, $v0  // multiply to get result
	jr    $ra            // and return
	nop

#pragma qtmips focus memory STACK_INIT-4*8
