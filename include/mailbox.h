extern volatile unsigned int mbox[36];

// Request/Response Codes
#define MAILBOX_REQUEST   0x00000000
#define MAILBOX_RESPONSE  0x80000000
#define MAILBOX_FULL      0x80000000
#define MAILBOX_EMPTY     0x40000000

// Channels
#define MAILBOX_CHANNEL_POWER_MANAGEMENT    0 
#define MAILBOX_CHANNEL_FRAMEBUFFER         1
#define MAILBOX_CHANNEL_VIRTUAL_UART        2
#define MAILBOX_CHANNEL_VCHIQ               3 
#define MAILBOX_CHANNEL_LEDS                4        
#define MAILBOX_CHANNEL_BUTTONS             5
#define MAILBOX_CHANNEL_TOUCH_SCREEN        6
#define MAILBOX_CHANNEL_COUNT               7
#define MAILBOX_CHANNEL_PROPERTY            8

// Tags
#define MAILBOX_TAG_SET_POWER_STATE     0x00028001
#define MAILBOX_TAG_SET_CLOCK_RATE      0x00038002

#define MAILBOX_TAG_SET_PHYSICAL_WH     0x00048003
#define MAILBOX_TAG_SET_VIRTUAL_WH      0x00048004
#define MAILBOX_TAG_SET_VIRTUAL_OFFSET  0x00048009
#define MAILBOX_TAG_SET_DEPTH           0x00048005
#define MAILBOX_TAG_SET_PIXEL_ORDER     0x00048006
#define MAILBOX_TAG_GET_FRAMEBUFFER     0x00040001
#define MAILBOX_TAG_GET_PITCH           0x00040008

#define MAILBOX_TAG_LAST                0x00000000



unsigned int allocate_memory(unsigned int size, unsigned int align, unsigned int flags);
unsigned int release_memory(unsigned int handle);
unsigned int lock_memory(unsigned int handle);
unsigned int unlock_memory(unsigned int handle);
unsigned int execute_code(unsigned int code, unsigned int r0, unsigned int r1, unsigned int r2, unsigned int r3, unsigned int r4, unsigned int r5);
unsigned int execute_qpu(unsigned int num_qpus, unsigned int control, unsigned int noflush, unsigned int timeout);
unsigned int qpu_enable(unsigned int enable);


void *mapmem(unsigned int base, unsigned int size);
void *unmapmem(void *addr, unsigned int size);


unsigned int mbox_call(unsigned char ch);

enum {
MEM_FLAG_DISCARDABLE = 1 << 0, /* can be resized to 0 at any time. Use for cached data */
MEM_FLAG_NORMAL = 0 << 2, /* normal allocating alias. Don't use from ARM */
MEM_FLAG_DIRECT = 1 << 2, /* 0xC alias uncached */
MEM_FLAG_COHERENT = 2 << 2, /* 0x8 alias. Non-allocating in L2 but coherent */
MEM_FLAG_L1_NONALLOCATING = (MEM_FLAG_DIRECT | MEM_FLAG_COHERENT), /* Allocating in L2 */
MEM_FLAG_ZERO = 1 << 4,  /* initialise buffer to all zeros */
MEM_FLAG_NO_INIT = 1 << 5, /* don't initialise (default is initialise to all ones */
MEM_FLAG_HINT_PERMALOCK = 1 << 6, /* Likely to be locked for long periods of time. */
};


