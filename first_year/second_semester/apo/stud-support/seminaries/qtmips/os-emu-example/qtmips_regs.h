// ******************************************************************
//  QtMips emulator https://github.com/cvut/QtMips support filees
//
//  qtmips_regs.h  - definition of the QtMips simulator peripherals
//
//  (C) Copyright 2019 - 2020 by Pavel Pisa
//      e-mail:   pisa@cmp.felk.cvut.cz
//      homepage: http://cmp.felk.cvut.cz/~pisa
//      license:  any combination of GPL, LGPL, MPL or BSD licenses
//
// ******************************************************************

#ifndef QTMIPS_REGS_H
#define QTMIPS_REGS_H

// Serial port/terminal registers
// There is mirror of this region at address 0xffff0000
// to match QtSpim and Mars emulators

#define SERIAL_PORT_BASE,      0xffffc000 // base address of serial port region

#define SERP_RX_ST_REG,        0xffffc000 // Receiver status register
#define SERP_RX_ST_REG_o,          0x0000 // Offset of RX_ST_REG
#define SERP_RX_ST_REG_READY_m,       0x1 // Data byte is ready to be read
#define SERP_RX_ST_REG_IE_m,          0x2 // Enable Rx ready interrupt

#define SERP_RX_DATA_REG,      0xffffc004 // Received data byte in 8 LSB bits
#define SERP_RX_DATA_REG_o,        0x0004 // Offset of RX_DATA_REG

#define SERP_TX_ST_REG,        0xffffc008 // Transmitter status register
#define SERP_TX_ST_REG_o,          0x0008 // Offset of TX_ST_REG
#define SERP_TX_ST_REG_READY_m,       0x1 // Transmitter can accept next byte
#define SERP_TX_ST_REG_IE_m,          0x2 // Enable Tx ready interrupt

#define SERP_TX_DATA_REG,      0xffffc00c // Write word to send 8 LSB bits to terminal
#define SERP_TX_DATA_REG_o,        0x000c // Offset of TX_DATA_REG


// Memory mapped peripheral for dial knobs input,
// LED and RGB LEDs output designed to match
// MZ_APO education Zynq based board developed
// by Petr Porazil and Pavel Pisa at PiKRON.com company

#define SPILED_REG_BASE       0xffffc100 // base of SPILED port region

#define SPILED_REG_LED_LINE   0xffffc104 // 32 bit word mapped as output
#define SPILED_REG_LED_LINE_o     0x0004 // Offset of the LED_LINE
#define SPILED_REG_LED_RGB1   0xffffc110 // RGB LED 1 color components
#define SPILED_REG_LED_RGB1_o     0x0010 // Offset of LED_RGB1
#define SPILED_REG_LED_RGB2   0xffffc114 // RGB LED 2 color components
#define SPILED_REG_LED_RGB2_o     0x0014 // Offset of LED_RGB2
#define SPILED_REG_KNOBS_8BIT 0xffffc124 // Three 8 bit knob values
#define SPILED_REG_KNOBS_8BIT_o   0x0024 // Offset of KNOBS_8BIT

// The simple 16-bit per pixel (RGB565) frame-buffer
// display size is 480 x 320 pixel
// Pixel format RGB565 expect
//   bits 11 .. 15 red component
//   bits  5 .. 10 green component
//   bits  0 ..  4 blue component
#define LCD_FB_START          0xffe00000
#define LCD_FB_END            0xffe4afff

// Mapping of interrupts
// Irq number   Cause/Status Bit   Source
//  2 / HW0      10                Serial port ready to accept character to Tx
//  3 / HW1      11                There is received character ready to be read
//  7 / HW5      15                Counter reached value in Compare register


#endif /*QTMIPS_REGS_H*/
