#ifndef	_P_MAILBOX_H
#define	_P_MAILBOX_H

#include "peripherals/p_base.h"

// Registers
#define MAILBOX_BASE        (PBASE + 0xB880)
#define MAILBOX_READ        (MAILBOX_BASE + 0x0000)
#define MAILBOX_POLL        (MAILBOX_BASE + 0x0010)
#define MAILBOX_SENDER      (MAILBOX_BASE + 0x0014)
#define MAILBOX_STATUS      (MAILBOX_BASE + 0x0018)
#define MAILBOX_CONFIG      (MAILBOX_BASE + 0x001C)
#define MAILBOX1_WRITE      (MAILBOX_BASE + 0x0020)

// Request/Response Codes
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

#endif  /*_P_MAILBOX_H */