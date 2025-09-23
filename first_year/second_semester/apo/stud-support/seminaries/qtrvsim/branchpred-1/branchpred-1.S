.globl _start

# i = s0, j = s1, c = s2, t0 = outer loop limits, t1 = inner loop limit, t3 - auxiliary variable
# c = 0; for ( i = 0; i < 4; i++) for ( j = 0; i < 5; j++) c++;
# c, i, j = 0; do { do { c++ } while(++j < 5) } while(i < 4);

_start:
  addi s2, zero, 0   # c = 0; total loop count 
  addi t0, zero, 4   # t0 = 4; outer loop limit
  addi t1, zero, 5   # t1 = 5; inner loop limit

  addi s0, zero, 0   # i = 0; outer loop inc
L1:
  addi s1, zero, 0   # j = 0; inner loop inc
L2:
  addi s2, s2, 1     # c++;

  addi s1, s1, 1     # j++;
  slt  t3, s1, t1    # t3 = (s1 < t1) ? 1 : 0;
  bne  t3, zero, L2
  addi s0, s0, 1     # i++;
  slt  t3, s0, t0    # t3 = (s0 < t0) ? 1 : 0;
  bne  t3, zero, L1
  ebreak
