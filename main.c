/*-
 * Copyright (c) 2019 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/cdefs.h>
#include <sys/console.h>
#include <sys/systm.h>
#include <sys/thread.h>
#include <sys/malloc.h>

#include <machine/cpuregs.h>
#include <machine/cpufunc.h>

#include <dev/uart/uart_16550.h>
#include <riscv/sifive/e300g_clint.h>

#define	CLINT_BASE		0x10000000
#define	UART_BASE		0x62300000
#define	UART_CLOCK_RATE		83000000
#define	DEFAULT_BAUDRATE	115200

static struct uart_16550_softc uart_sc;
static struct clint_softc clint_sc;

extern uint32_t _sbss;
extern uint32_t _ebss;

static void
uart_putchar(int c, void *arg)
{
	struct uart_16550_softc *sc;

	sc = arg;

	if (sc == NULL)
		return;

	if (c == '\n')
		uart_16550_putc(sc, '\r');

	uart_16550_putc(sc, c);
}

static void
clear_bss(void)
{
	uint8_t *sbss;
	uint8_t *ebss;

	sbss = (uint8_t *)&_sbss;
	ebss = (uint8_t *)&_ebss;

	while (sbss < ebss)
		*sbss++ = 0;
}

static void
test_thr(void *arg)
{

	while (1) {
		critical_enter();
		printf("Hello from thread %d\n", (int)arg);
		critical_exit();
	}
}

void
main(void)
{

	clear_bss();

	md_init();

	uart_16550_init(&uart_sc, UART_BASE,
	    UART_CLOCK_RATE, DEFAULT_BAUDRATE);
	console_register(uart_putchar, (void *)&uart_sc);

	printf("Hello World\n");

	malloc_init();
	malloc_add_region(0x80040000, 0x40000);

	e300g_clint_init(&clint_sc, CLINT_BASE);

	thread_create("test", 1, 10000, 4096, test_thr, (void *)1);
	thread_create("test", 1, 10000, 4096, test_thr, (void *)2);

	intr_enable();

	__asm __volatile("ecall");

	while (1)
		cpu_idle();
}
