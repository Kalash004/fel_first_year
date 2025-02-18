	li   $s1, 0x11111111
	li   $s2, 0x22222222
	li   $s3, 0x33333333
	li   $s4, 0x44444444
	li   $s5, 0x55555555

	lw   $s0, 40($0)
	and  $t0, $s0, $s1
	or   $t1, $s4, $s0
	sub  $t2, $s0, $s5

.org 40
	.word 0x12345678
