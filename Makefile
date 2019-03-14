APP =		piccolo-demo
ARCH =		riscv

CC =		${CROSS_COMPILE}gcc
LD =		${CROSS_COMPILE}ld
OBJCOPY =	${CROSS_COMPILE}objcopy

LDSCRIPT =	${.CURDIR}/ldscript

OBJECTS =	main.o						\
		osfive/sys/dev/uart/uart_16550.o		\
		osfive/sys/kern/kern_malloc.o			\
		osfive/sys/kern/kern_malloc_fl.o		\
		osfive/sys/kern/kern_panic.o			\
		osfive/sys/kern/kern_sched.o			\
		osfive/sys/kern/kern_timeout.o			\
		osfive/sys/kern/subr_prf.o			\
		osfive/sys/kern/subr_console.o			\
		osfive/sys/riscv/riscv/exception.o		\
		osfive/sys/riscv/riscv/machdep.o		\
		osfive/sys/riscv/riscv/trap.o			\
		start.o

LIBRARIES =	LIBC

CFLAGS =	-O -pipe -g -nostdinc -fno-omit-frame-pointer		\
	-march=rv32im -mabi=ilp32 -fno-builtin-printf			\
	-fno-optimize-sibling-calls -ffreestanding -fwrapv		\
	-fdiagnostics-show-option -fms-extensions -finline-limit=8000	\
	-Wall -Wredundant-decls -Wnested-externs -Wstrict-prototypes	\
	-Wmissing-prototypes -Wpointer-arith -Winline -Wcast-qual	\
	-Wundef -Wno-pointer-sign -Wno-format -Wmissing-include-dirs	\
	-Wno-unknown-pragmas -Werror

all:	__compile __link __binary

clean:	__clean

.include "osfive/lib/libc/Makefile.inc"
.include "osfive/mk/bsd.mk"

.ifndef CROSS_COMPILE
.error Error: CROSS_COMPILE is not set
.endif
