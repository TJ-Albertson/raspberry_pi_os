#include "utils.h"
#include "mailbox.h"
#include "peripherals/p_mailbox.h"

#include <stdint.h>

// The buffer must be 16-byte aligned as only the upper 28 bits of the address can be passed via the mailbox
volatile unsigned int __attribute__((aligned(16))) mbox[36];

unsigned int mbox_call(unsigned char ch)
{
    // 28-bit address (MSB) and 4-bit value (LSB)
    unsigned int r = ((unsigned int)((long) &mbox) &~ 0xF) | (ch & 0xF);

    // Wait until we can write
    while (get32(MAILBOX_STATUS) & MAILBOX_FULL);
    
    // Write the address of our buffer to the mailbox with the channel appended
    put32(MAILBOX1_WRITE, r);

    while (1) {
        // Is there a reply?
        while (get32(MAILBOX_STATUS) & MAILBOX_EMPTY);

        // Is it a reply to our message?
        if (r == get32(MAILBOX_READ)) 
            return mbox[1] == MAILBOX_RESPONSE; // Is it successful?
           
    }
    return 0;
}


// V3D Calls
unsigned int qpu_enable(unsigned int enable)
{
   // Buffer Size
   mbox[0] = 0;              // size

   // Request/Response Code
   mbox[1] = MAILBOX_REQUEST;     // process request

   // Start of tags
   mbox[2] = 0x30012;        // (the tag id)     my_note: Undocumented???
   mbox[3] = 4;              // (size of the buffer)
   mbox[4] = 4;              // (size of the data)
   mbox[5] = enable;

   mbox[6] = MAILBOX_TAG_LAST;     // end tag

   if (mbox_call(MAILBOX_CHANNEL_PROPERTY)) {
      return mbox[5];
   }
}

// Allocates contiguous memory on the GPU. size and alignment are in bytes.
unsigned int allocate_memory(unsigned int size, unsigned int align, unsigned int flags)
{
   mbox[0] = 0;              // size
   mbox[1] = MAILBOX_REQUEST;     // process request

   mbox[2] = 0x3000c;        // (the tag id)
   mbox[3] = 12;             // (size of the buffer)
   mbox[4] = 12;             // (size of the data)
   mbox[5] = size;           // (num bytes? or pages?)
   mbox[6] = align;          // (alignment)
   mbox[7] = flags;          // (MEM_FLAG_L1_NONALLOCATING)

   mbox[8] = MAILBOX_TAG_LAST;     // end tag

   if (mbox_call(MAILBOX_CHANNEL_PROPERTY)) {
      return mbox[5];
   }
}

// Lock buffer in place, and return a bus address. Must be done before memory can be accessed. bus address != 0 is success.
unsigned int lock_memory(unsigned int handle)
{
   mbox[0] = 0;              // size
   mbox[1] = MAILBOX_REQUEST;     // process request

   mbox[2] = 0x3000d;        // (the tag id)
   mbox[3] = 4;              // (size of the buffer)
   mbox[4] = 4;              // (size of the data)
   mbox[5] = handle;

   mbox[6] = MAILBOX_TAG_LAST;     // end tag

   if (mbox_call(MAILBOX_CHANNEL_PROPERTY)) {
      return mbox[5];
   }
}

// Unlock buffer. It retains contents, but may move. Needs to be locked before next use. status=0 is success.
unsigned int unlock_memory(unsigned int handle)
{
   mbox[0] = 0;              // size
   mbox[1] = MAILBOX_REQUEST;     // process request

   mbox[2] = 0x3000e;        // (the tag id)
   mbox[3] = 4;              // (size of the buffer)
   mbox[4] = 4;              // (size of the data)
   mbox[5] = handle;

   mbox[6] = MAILBOX_TAG_LAST;     // end tag

    if (mbox_call(MAILBOX_CHANNEL_PROPERTY)) {
      return mbox[5];
   }
}

// Free the memory buffer. status=0 is success.
unsigned int release_memory(unsigned int handle)
{
   mbox[0] = 0;              // size
   mbox[1] = MAILBOX_REQUEST;     // process request

   mbox[2] = 0x3000f;        // (the tag id)
   mbox[3] = 4;              // (size of the buffer)
   mbox[4] = 4;              // (size of the data)
   mbox[5] = handle;

   mbox[6] = MAILBOX_TAG_LAST;     // end tag

   if (mbox_call(MAILBOX_CHANNEL_PROPERTY)) {
      return mbox[5];
   }
}

// Not in documentation
unsigned int execute_qpu(unsigned int num_qpus, unsigned int control, unsigned int noflush, unsigned int timeout) 
{
   mbox[0] = 0;              // size
   mbox[1] = MAILBOX_REQUEST;     // process request

   mbox[2] = 0x30011;        // (the tag id)
   mbox[3] = 16;             // (size of the buffer)
   mbox[4] = 16;             // (size of the data)
   mbox[5] = num_qpus;
   mbox[6] = control;
   mbox[7] = noflush;
   mbox[8] = timeout;        // ms

   mbox[9] = 0x00000000;     // end tag

   if (mbox_call(MAILBOX_CHANNEL_PROPERTY)) {
      return mbox[5];
   }
}

void *mapmem(unsigned base, unsigned size)
{

}

void *unmapmem(void *addr, unsigned size)
{

}