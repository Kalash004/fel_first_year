    .option norelax
    .globl    array_size
    .globl    array_start
    .text
    .globl _start

    _start:
        la   a0, array_start  
        la   a1, array_size
        lw   a1, 0(a1)

        addi x7, a1, -1 /* N - 1 */

    forout:
        addi x1, x0, 0 /* j */
        addi x2, x7, 0 /* N - 1 */
        add x3, a0, x0 /* N - 1 - i */
    forin:
        lw x4, 0(x3) /* arr[j] */
        lw x5, 4(x3) /* arr[j + 1] */
        ble x4, x5, skip

        sw x5, 0(x3)
        sw x4, 4(x3)


    skip:    
        addi x3, x3, 4 /* j++ */
        addi x1, x1, 1
        blt  x1, x2, forin

        addi x7, x7, -1
        bgtz x7, forout



        /* space for your code */

    loop:    ebreak
            beq     zero, zero, loop

    .data

    array_size:
    .word    15
    array_start:
    .word    5, 3, 4, 1, 15, 8, 9, 2, 10, 6, 11, 1, 6, 9, 12