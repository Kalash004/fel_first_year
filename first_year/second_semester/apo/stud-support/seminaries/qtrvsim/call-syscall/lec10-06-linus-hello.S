// The simple program to run on RISC-V Linux kernel port

.globl  _start

// select core without pipeline but with delay slot

#pragma qtrvsim show registers
#pragma qtrvsim show memory
#pragma qtrvsim show terminal

// #include <asm/unistd.h>
// #include <asm/asm.h>
// #include <sys/syscall.h>

.equ O_RDWR,       02

// Linux kernel compatible system calls subset

// Linux kernel compatible system calls subset

.equ __NR_exit,        93  // void exit(int status)
.equ __NR_read,        63  // ssize_t read(int fd, void *buf, size_t count)
.equ __NR_write,       64  // ssize_t write(int fd, const void *buf, size_t count)
.equ __NR_close,       57  // int close(int fd)
.equ __NR_openat,      56  // int openat(int fd, const char *pathname, int flags, mode_t mode)
	// use fd = -100 for normal open behaviour. Full openat not supported.
.equ __NR_brk,         214 // void * brk(void *addr)
.equ __NR_ftruncate64, 46  // int ftruncate64(int fd, off_t length)
.equ __NR_readv,       65  // ssize_t readv(int fd, const struct iovec *iov, int iovcnt)
.equ __NR_writev,      66  // ssize_t writev(int fd, const struct iovec *iov, int iovcnt)

.option norelax
.text

_start:
main:
//	LEAF(main)
//	fd = openat(AT_EMPTY_PATH, "/dev/tty1", O_RDWR, 0);
	addi    a0, zero, -100
	la      a1, tty
	li      a2, O_RDWR
	li      a3, 0
	li      a7, __NR_openat
	ecall
	blt     a0, zero, quit
	addi    s0, a0, 0         // delay slot
//	write(fd, "hello, world.\n", 14);
	addi    a0, s0, 0
	la      a1, hello
	li      a2, 14
	li      a7, __NR_write
	ecall
//	close(fd);
	addi    a0, s0, 0
	li      a7, __NR_close
	ecall

quit:
	li      a0, 0
	li      a7, __NR_exit
	ecall

	ebreak
	j       quit
	nop

//	END(main)

	.data
tty:    .asciz  "/dev/tty"
hello:  .ascii  "Hello, world.\n"
