#ifndef __RPMSGLINK_H__
#define __RPMSGLINK_H__

#include <stdbool.h>
#include <stdint.h>
#include "crtp.h"

#define RPMSG_RX_TX_PACKET_SIZE   (64)

// Structure for RPMsg packets
typedef struct USBPacket USBPacket;

// Public functions
void rpmsglinkInit();
bool rpmsglinkTest();
struct crtpLinkOperations * rpmsglinkGetLink();

#endif /* __RPMSGLINK_H__ */