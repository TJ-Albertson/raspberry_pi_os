// Sets all values in bss section to zero. X0 = bss_begin and X1 is the size of bss section.
.globl memzero
memzero:
	str xzr, [x0], #8     // Store XZR (Zero Reg) onto address X0 then increment X0 address by 8
	subs x1, x1, #8       // Subtract and set flags. X1 = X1 - 8. 
	b.gt memzero          // Branch if greater than. Will jump to memzero label if result of SUBS was greater than 0.
	ret 			      // return
