#ifndef	_P_MAILBOX_H
#define	_P_MAILBOX_H

#include "peripherals/p_base.h"

#define MAILBOX_BASE    (PBASE+0x0000B880)

#define MAIL0_READ      (MAILBOX_BASE+0x0000)
#define MAIL0_STATUS    (MAILBOX_BASE+0x0018)
#define MAIL0_WRITE     (MAILBOX_BASE+0x0020)

#define PROPERTY_CHANNEL    8
#define FRAMEBUFFER_CHANNEL 1

#endif  /*_P_MAILBOX_H */