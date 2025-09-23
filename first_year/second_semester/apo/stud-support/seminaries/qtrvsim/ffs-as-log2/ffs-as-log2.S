// ffs as log2(x)
.globl _start
.globl __start
.option norelax
.text

__start:
_start:
	addi	a0, zero, 157	// int x = 157;
	addi	t1, zero, -1	// int y = 0;
	beq	a0, zero, done	// while(x != 0)
loop:				// {
	srli	a0, a0, 1		// x = x / 2;
	addi	t1, t1, 1		// y = y + 1;
	bne	a0, zero, loop	//}
done:
	addi	a0, t1, 0
finish:	ebreak			// stop execution
	beq	zero, zero, finish	// loop to catch continue
