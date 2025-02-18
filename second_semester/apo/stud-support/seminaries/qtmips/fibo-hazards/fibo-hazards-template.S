// fibo-hazards.S file template, rename and implement the algorithm
// Test algorithm in qtmips_gui program
// Select the CPU core configuration to
//    Pipelined without hazard unit and cache
// This option select version where RAW dependencies which leads
// to hazards are not resolved in hardware, you need to schedule
// instruction execution according to the pipeline structure
// (classical 5-stage MIPS) such way, that no dependency results
// in a hazard


// copy directory with the project to your repository to
// the directory work/fibo-hazards
// critical is location of the file work/fibo-hazards/fibo-hazards.S
// which is checked by the scripts

// The script loads number of the last Fibonacci series element to compute
// into fibo_limit variable and expects computed series in memory starting
// at address fibo_series, the series has to be followed by at least
// one zero element

// When tested by actual qtmips_cli version, the variant without hazard
// unit cannot be selected (this is WIP for the test script), use qtmips_gui
// which is fully configurable

// Directives to make interesting windows visible
#pragma qtmips show registers
#pragma qtmips show memory

.set noreorder
.set noat

.globl    fibo_limit
.globl    fibo_series

.text
.globl _start
.ent _start

_start:

	la   $a0, fibo_series
	la   $a1, fibo_limit
	lw   $a1, 0($a1) // number of elements in the array

//Insert your code there

//Final infinite loop
end_loop:
	cache 9, 0($0)  // flush cache memory
	break           // stop the simulator
	j end_loop
	nop

.end _start

.data
// .align    2 // not supported by QtMips yet

fibo_limit:
	.word 15
fibo_series:
	.skip 1000*4

// Specify location to show in memory window
#pragma qtmips focus memory fibo_limit
