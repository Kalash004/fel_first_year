	li   s1, 0x11111111
	li   s2, 0x22222222
	li   s3, 0x33333333
	li   s4, 0x44444444
	li   s5, 0x55555555
	addi t1, zero, 0x123
	addi t2, zero, 0x123

	beq  t1, t2, skip
	and  t0, s0, s1
	or   t1, s4, s0
	sub  t2, s0, s5

skip:
	slt  t3, s2, s3
