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
	li   $s0, 0x12345678
	addi $a0, $0, 2
	addi $a1, $0, 3
	addi $a2, $0, 4
	addi $a3, $0, 5
	jal  fun
	nop
	add  $t0, $v0, $0
	sw   $t0, SPILED_REG_LED_LINE($0)
final:
	break
	beq  $0, $0, final

// int fun(int g, h, i, j)
// g→$a0, h→$a1, i→$a2, j→$a3, $v0 – ret. val, $sX – save, $tX – temp, $ra – ret. addr
// return (g + h) – (i + j)
fun:
	addi $sp, $sp, -4
	sw   $s0, 0($sp)	 // Save $s0 on stack
	add  $t0, $a0, $a1
	add  $t1, $a2, $a3	 // Procedure body
	sub  $s0, $t0, $t1
	add  $v0, $s0, $zero // Result
	lw   $s0, 0($sp)	 // Restore $s0
	addi $sp, $sp, 4
	jr   $ra		 // Return
	nop

#pragma qtmips focus memory STACK_INIT-4*8
