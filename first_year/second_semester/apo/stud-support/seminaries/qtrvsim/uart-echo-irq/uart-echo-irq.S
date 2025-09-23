.globl  _start
.option norelax

// Serial port/terminal registers
// There is mirror of this region at address 0xffff0000
// to match QtSpim and Mars emulators

.equ SERIAL_PORT_BASE,      0xffffc000 // base address of serial port region

.equ SERP_RX_ST_REG,        0xffffc000 // Receiver status register
.equ SERP_RX_ST_REG_o,          0x0000 // Offset of RX_ST_REG
.equ SERP_RX_ST_REG_READY_m,       0x1 // Data byte is ready to be read
.equ SERP_RX_ST_REG_IE_m,          0x2 // Enable Rx ready interrupt

.equ SERP_RX_DATA_REG,      0xffffc004 // Received data byte in 8 LSB bits
.equ SERP_RX_DATA_REG_o,        0x0004 // Offset of RX_DATA_REG

.equ SERP_TX_ST_REG,        0xffffc008 // Transmitter status register
.equ SERP_TX_ST_REG_o,          0x0008 // Offset of TX_ST_REG
.equ SERP_TX_ST_REG_READY_m,       0x1 // Transmitter can accept next byte
.equ SERP_TX_ST_REG_IE_m,          0x2 // Enable Tx ready interrupt

.equ SERP_TX_DATA_REG,      0xffffc00c // Write word to send 8 LSB bits to terminal
.equ SERP_TX_DATA_REG_o,        0x000c // Offset of TX_DATA_REG


// Memory mapped peripheral for dial knobs input,
// LED and RGB LEDs output designed to match
// MZ_APO education Zynq based board developed
// by Petr Porazil and Pavel Pisa at PiKRON.com company

.equ SPILED_REG_BASE,       0xffffc100 // base of SPILED port region

.equ SPILED_REG_LED_LINE,   0xffffc104 // 32 bit word mapped as output
.equ SPILED_REG_LED_LINE_o,     0x0004 // Offset of the LED_LINE
.equ SPILED_REG_LED_RGB1,   0xffffc110 // RGB LED 1 color components
.equ SPILED_REG_LED_RGB1_o,     0x0010 // Offset of LED_RGB1
.equ SPILED_REG_LED_RGB2,   0xffffc114 // RGB LED 2 color components
.equ SPILED_REG_LED_RGB2_o,     0x0014 // Offset of LED_RGB2
.equ SPILED_REG_KNOBS_8BIT, 0xffffc124 // Three 8 bit knob values
.equ SPILED_REG_KNOBS_8BIT_o,   0x0024 // Offset of KNOBS_8BIT

// simolified subsed of the Task Control Block
.equ    TCB_SP, 0                       // task stack pointer
.equ    TCB_RA, 4                       // task return address
.equ    TCB_T0, 8
.equ    TCB_A0, 12

.text

_start: 
        addi    a0, zero, 0x101
        li      t0, 0x00001800
        // ensure to stay in machine mode after mret test
        csrrw   zero, mstatus, t0
        la      t0, skip
        csrrw   zero, mepc, t0
        mret    // test exception ret

        addi    a0, zero, 0x105
        addi    a0, zero, 0x106
skip:
        addi    a0, zero, 0x107
        csrrs   t0, mepc, zero

        ebreak

        la      t0, handle_exception
        csrrw   zero, mtvec, t0

        la      t0, task_control_block
        csrrw   zero, mscratch, t0

        csrrsi   zero,mstatus,8 //MIE=1

        addi    t0, zero, 16 // UART RX
        addi    t1, zero, 1
        sll     t1, t1, t0  // bit mask
        csrrs   zero, mie, t1

        li      a0, SERIAL_PORT_BASE
        li      t0, SERP_RX_ST_REG_IE_m
        sw      t0, SERP_RX_ST_REG_o(a0)

        // Background task
        addi    t0, zero, 0x0001
        li      a0, SPILED_REG_BASE
loop:
        sw      t0, SPILED_REG_LED_LINE_o(a0)
        srli    t2, t0, 31
        slli    t0, t0, 1
        or      t0, t0, t2
        lw      t2, SPILED_REG_KNOBS_8BIT_o(a0)
        sw      t2, SPILED_REG_LED_RGB1_o(a0)
        xori    t2, t2, -1
        sw      t2, SPILED_REG_LED_RGB2_o(a0)
        beq     zero, zero, loop

handle_exception:
        csrrw   tp, mscratch, tp                // store previous and take system tp
        sw      sp, TCB_SP(tp)                  // store stack pointer
        sw      ra, TCB_RA(tp)                  // store return address
        sw      t0, TCB_T0(tp)                  // store rest of clobberable regs
        sw      a0, TCB_A0(tp)
        //...
        csrr    t0, mcause                              // is it Rx interrupt?
        blt             t0, zero, handle_irq    // branch to interrupts processing
        //...
        // handle synchronous exception

ret_from_exception:
        lw      sp, TCB_SP(tp)                  // restore stack pointer
        lw      ra, TCB_RA(tp)                  // restore return address
        lw      t0, TCB_T0(tp)                  // restore rest of clobberable regs
        lw      a0, TCB_A0(tp)
        ///...
        csrrw   tp, mscratch, tp                // Swap back TCB to mscratch
        mret                                    // Return from exception pc <= mepc

handle_irq:                                     // t0 mcause
        slli    t0, t0, 2                       // shift out sign, left sources * 4
        // the t0 would be used to point into irq handlers table
        // check only for UART RX interupt for simplicity
        addi    a0, zero, 16 * 4                // UART RX is the first platform irq
        beq     t0, a0, handle_uart_rx_irq      // it is UART RX
        // mask out unknown sources
        srli    t0, t0, 2                       // make t0 back simple source index
        addi    a0, zero, 1
        sll     a0, a0, t0                      // generate bit mask for source
        csrrc   zero, mie, a0                   // mie = mie & ~a0
        j       ret_from_exception

handle_uart_rx_irq:
        li      a0, SERIAL_PORT_BASE            // Setup base of UART
        lw      t0, SERP_RX_DATA_REG_o(a0)      // Read received character
        sw      t0, SERP_TX_DATA_REG_o(a0)      // echo it back to terminal
        j       ret_from_exception

.data

task_control_block:
        .skip   64
