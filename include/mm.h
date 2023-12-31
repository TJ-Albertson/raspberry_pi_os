#ifndef _MM_H
#define _MM_H

#define PAGE_SHIFT      12
#define TABLE_SHIFT     9
#define SECTION_SHIFT   (PAGE_SHIFT + TABLE_SHIFT)

// 1 << 12 == 0001 0000 0000 0000 == 4KB
#define PAGE_SIZE       (1 << PAGE_SHIFT)

// 1 << 21 == 0001 0000 0000 0000 0000 0000 == 2MB
#define SECTION_SIZE    (1 << SECTION_SHIFT)

// 4MB
#define LOW_MEMORY      (2 * SECTION_SIZE)

#ifndef __ASSEMBLER__

void memzero(unsigned long src, unsigned long n);

#endif

#endif /*_MM_H */
