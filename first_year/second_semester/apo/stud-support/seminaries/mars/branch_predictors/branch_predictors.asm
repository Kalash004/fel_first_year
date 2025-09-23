start:
	addi $v0, $zero, 0	# cycles = 0

	addi $s1, $zero, 3
L1: # for (i = 3; i !=0; i--)
	addi $s2, $zero, 5	# i = 3
L2: # for (j = 5; j !=0; j--)
	addi $s3, $zero, 4
L3: # for (k = 4; k !=0; k--)
	addi $v0, $v0, 1	# cycles++
	addi $s3, $s3, -1	# k--
	bne  $s3, $zero, L3	# if (k != 0) goto L3

	addi $s2, $s2, -1	# j--
	bne  $s2, $zero, L2	# if (j != 0) goto L2

	addi $s1, $s1, -1	# i--
	bne  $s1, $zero, L1	# if (i != 0) goto L1

	break
