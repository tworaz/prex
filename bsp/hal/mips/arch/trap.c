/*-
 * Copyright (c) 2011, Peter Tworek
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
 * 3. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
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

/*
 * trap.c - called from the abort handler when a processor detect abort.
 */

#include <conf/config.h>
#include <mips/cpuregs.h>
#include <sys/signal.h>
#include <kernel.h>
#include <task.h>
#include <hal.h>
#include <exception.h>
#include <interrupt.h>
#include <cpu.h>
#include <trap.h>
#include <cpufunc.h>
#include <context.h>

#if 0
#ifdef DEBUG
/*
 * Trap name
 */
static char *const trap_name[] = {
	"Undefined Instruction",
	"Prefetch Abort",
	"Data Abort"
};
#define MAXTRAP (sizeof(trap_name) / sizeof(void *) - 1)

#endif	/* !DEBUG */

/*
 * abort/exception mapping table.
 * MIPS exception is translated to the architecture
 * independent exception code.
 */
static const int exception_map[] = {
	SIGILL,		/* Undefined instruction */
	SIGSEGV,	/* Prefech abort */
	SIGSEGV,	/* Data abort */
};
#endif

#ifdef DEBUG
# define DEBUG_TRAP(r, c, v) trap_dump(r, c, v)
#else
# define DEBUG_TRAP(r, c, v)
#endif

/*
 * Forward declarations
 */
register_t
syscall_handler(register_t a0, register_t a1, register_t a2, register_t a3,
                register_t id);

/*
 * Trap handler
 * Invoke the exception handler if it is needed.
 */
void
trap_handler(struct cpu_regs *r)
{
	uint32_t code;
	uint32_t is_kern;
	uint32_t cause = get_cp0_cause();
	uint32_t vaddr = get_cp0_badvaddr();

	code = (cause & MIPS_CAUSE_EXC_CODE_MASK) >> MIPS_CAUSE_EXC_CODE_SHIFT;
	is_kern = (r->status & MIPS_STATUS_KSU_MASK) == MIPS_STATUS_KSU_KERNEL;

	/* Interrupts and syscalls should never go here */
	ASSERT(code != MIPS_CAUSE_EXC_CODE_IRQ);
	ASSERT(code != MIPS_CAUSE_EXC_CODE_SYS);

	switch(code) {
	case MIPS_CAUSE_EXC_CODE_TLBL:
	case MIPS_CAUSE_EXC_CODE_TLBS:
		DEBUG_TRAP(r, cause, vaddr);
		panic("TLB exception in kernel\n");
		exception_mark(SIGSEGV);
		break;

	case MIPS_CAUSE_EXC_CODE_RI:
		DEBUG_TRAP(r, cause, vaddr);
		exception_mark(SIGILL);
		break;

	default:
		DEBUG_TRAP(r, cause, vaddr);
		panic("Unhandled exception type!");
	}

	exception_deliver();

	/* Disable interrupts */
	splhigh();
#if 0
	u_long trap_no = regs->r0;

	if ((regs->cpsr & PSR_MODE) == PSR_SVC_MODE &&
	    trap_no == TRAP_DATA_ABORT &&
	    (regs->pc - 4 == (uint32_t)known_fault1 ||
	     regs->pc - 4 == (uint32_t)known_fault2 ||
	     regs->pc - 4 == (uint32_t)known_fault3)) {
		DPRINTF(("\n*** Detect Fault! address=%x task=%s ***\n",
			 get_faultaddress(), curtask->name));
		regs->pc = (uint32_t)copy_fault;
		return;
	}
#ifdef DEBUG
	printf("=============================\n");
	printf("Trap %x: %s\n", (u_int)trap_no, trap_name[trap_no]);
	if (trap_no == TRAP_DATA_ABORT)
		printf(" Fault address=%x\n", get_faultaddress());
	else if (trap_no == TRAP_PREFETCH_ABORT)
		printf(" Fault address=%x\n", regs->pc);
	printf("=============================\n");

	trap_dump(regs);
	for (;;) ;
#endif
	if ((regs->cpsr & PSR_MODE) != PSR_USR_MODE)
		panic("Kernel exception");

	exception_mark(exception_map[trap_no]);
	exception_deliver();
#endif
}

#ifdef DEBUG
void
trap_dump(struct cpu_regs *r, uint32_t cause, uint32_t vaddr)
{
	printf("\nTrap frame %x, task=%s\n", r, curtask->name);

	printf(" at %08x v0 %08x v1 %08x a0 %08x a1 %08x a2 %08x\n",
	       r->at, r->v0, r->v1, r->a0, r->a1, r->a2);
	printf(" a3 %08x t0 %08x t1 %08x t2 %08x t3 %08x t4 %08x\n",
	       r->a3, r->t0, r->t1, r->t2, r->t3, r->t4);
	printf(" t5 %08x t6 %08x t7 %08x s0 %08x s1 %08x s2 %08x\n",
	       r->t5, r->t6, r->t7, r->s0, r->s1, r->s2);
	printf(" s3 %08x s4 %08x s5 %08x s6 %08x s7 %08x t8 %08x\n",
	       r->s3, r->s4, r->s5, r->s6, r->s7, r->t8);
	printf(" t9 %08x gp %08x sp %08x s8 %08x ra %08x\n",
	       r->t9, r->gp, r->sp, r->s8, r->ra);

	printf("\n epc %08x  bad %08x  status %08x  cause %08x\n\n",
	       r->epc, vaddr, r->status, cause);
}
#endif /* !DEBUG */
