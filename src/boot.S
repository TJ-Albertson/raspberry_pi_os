#include "mm.h"
#include "arm/sysregs.h"

.section ".text.boot"       // This code will be in the .text.boot section set by the linker file

// Code runs on every processor by default. Because a lot of sequential task need to be done it is best to run on one core for startup.
// The start function will check the processor id and put it in an infinite loop if it is not the first core.
.globl _start
_start:
	mrs	x0, mpidr_el1		// Copy MPIDR_EL1 to XO. the purpose of MPIDR_EL1 is processor identification (Multiprocessor Affinity Register).
	and	x0, x0, #0xFF		// And #OxFF and X0. Saves back to X0. This will zero out all but the first byte of X0. The first byte represents Aff0 aka Core #.
	cbz	x0, master		    // Compare and Branch on Zero with X0. If X0 == 0 go to master. 
	b	infinite_loop       // Go to infinite loop label. 

infinite_loop: 
	b 	infinite_loop

// Exception Level Settings
// Specific bit manipulations in /include/arm/sysregs.h
master:
    // SCTLR_EL1, System Control Register (EL1)
	// Provides top level control of the system, including its memory system, at EL1 and EL0.
	ldr	x0, =SCTLR_VALUE_MMU_DISABLED
	msr	sctlr_el1, x0		

	// HCR_EL2, Hypervisor Configuration Register (EL2)
	// Provides configuration controls for virtualization, including defining whether various operations are trapped to EL2.
	ldr	x0, =HCR_VALUE
	msr	hcr_el2, x0

	// SCR_EL3, Secure Configuration Register (EL3)
	// The Security state of EL0, EL1, and EL2. The Security state is either Secure or Non-secure.
	ldr	x0, =SCR_VALUE
	msr	scr_el3, x0

    // SPSR_EL3, Saved Program Status Register (EL3) Page 389 of AArch64-Reference-Manual.
	// Holds the saved process state when an exception is taken to EL3.
	ldr	x0, =SPSR_VALUE
	msr	spsr_el3, x0


	// allow simd, floating point
	ldr x0, =CPACR_EL1_MASK
	msr cpacr_el1, x0


	// elr_el3 holds the address, to which we are going to return after eret instruction will be executed
	adr	x0, el1_entry		
	msr	elr_el3, x0

	eret	

el1_entry:
	adr	x0, bss_begin       // X0 == bss_begin. bss_begin is the start address of the bss section set by linker. The bss section contains unintializied variables from your code.
	adr	x1, bss_end         // X1 == bss_end.
	sub	x1, x1, x0			// X1 = X1 - X0. Aka the size of the bss section. 
	bl 	memzero             // Branch to memzero and save return address in link register (lr/X30)

	mov	sp, #LOW_MEMORY     // Set stack pointer to #LOW_MEMORY macro
	bl	kernel_main         // Branch to kernel_main and save return address in link register (lr/X30)
	b 	infinite_loop		// should never come here
