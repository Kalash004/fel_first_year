// The first program run by Linus on MIPS Linux kernel port


// select core without pipeline but with delay slot

#pragma qtmips show registers
#pragma qtmips show memory
#pragma qtmips show terminal

// #include <asm/unistd.h>
// #include <asm/asm.h>
// #include <sys/syscall.h>

.equ O_RDWR,       02

// Linux kernel compatible system calls subset

.equ SYS_exit,     4001	// void exit(int status)
.equ SYS_read,     4003	// ssize_t read(int fd, void *buf, size_t count)
.equ SYS_write,    4004	// ssize_t write(int fd, const void *buf, size_t count)
.equ SYS_close,    4006	// int close(int fd)
.equ SYS_open,     4005	// int open(const char *pathname, int flags, mode_t mode)
.equ SYS_brk,      4045	// void * brk(void *addr)
.equ SYS_truncate, 4092	// int ftruncate(int fd, off_t length)
.equ SYS_readv,    4145	// ssize_t readv(int fd, const struct iovec *iov, int iovcnt)
.equ SYS_writev,   4146	// ssize_t writev(int fd, const struct iovec *iov, int iovcnt)

.set    noreorder
.text

//      LEAF(main)
//      fd = open("/dev/tty1", O_RDWR, 0);
        la      $a0, tty
        li      $a1, O_RDWR
        li      $a2, 0
        li      $v0, SYS_open
        syscall
        bne     $a3, $0, quit
        addi    $s0, $v0, 0         // delay slot
//      write(fd, "hello, world.\n", 14);
        addi    $a0, $s0, 0
        la      $a1, hello
        li      $a2, 14
        li      $v0, SYS_write
        syscall
//      close(fd);
        addi    $a0, $s0, 0
        li      $v0, SYS_close
        syscall

quit:
        li      $a0, 0
        li      $v0, SYS_exit
        syscall

        j       quit
        nop

//      END(main)

        .data
tty:    .asciz  "/dev/tty1"
hello:  .ascii  "Hello, world.\n"
