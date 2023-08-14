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

#endif  /*_P_MAILBOX_H */