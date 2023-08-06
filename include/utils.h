#ifndef	_BOOT_H
#define	_BOOT_H

// delay(unsigned long num_of_iterations). SUBS until zero
extern void delay ( unsigned long);
// Put32 (unsigned long data, unsigned int address)
extern void put32 ( unsigned long, unsigned int );
// Get32 (unsigned long address)
extern unsigned int get32 ( unsigned long );
// Returns current exception level
extern int get_el ( void );

#endif  /*_BOOT_H */
