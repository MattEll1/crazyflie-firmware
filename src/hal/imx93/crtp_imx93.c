/**
 * i.MX93 implementation of CRTP protocol
 */
 #include <stdbool.h>
 #include <stddef.h>
 #include "crtp.h"
 #include "config.h"
 #include "FreeRTOS.h"
 #include "semphr.h"
 #include "task.h"
 #include "queue.h"
 
 /* Only compile these functions if original CRTP is not included */
#ifdef CRAZYFLIE_HITL_MODE_ONLY
 /* CRTP implementations for i.MX93 */
 int crtpSendPacket(CRTPPacket *p)
 {
   // For initial port, return success (1) but don't actually send
   // This can be enhanced later with actual implementation
   return 1; // Return 1 for success, according to the API
 }
 
 int crtpGetFreeTxQueuePackets(void)
 {
   // For initial port, return 1 to indicate space is available
   // This can be enhanced later with actual implementation
   return 1;
 }

 void crtpInit(void)
{
  // Initialize CRTP subsystem - stub for now
}

bool crtpTest(void)
{
  // Return true to indicate CRTP is working
  return true;
}

int crtpReceivePacket(CRTPPort taskId, CRTPPacket *p)
{
  // Stub implementation - no packets to receive yet
  return 0; // Return 0 for no packet available
}

#endif /* CRAZYFLIE_HITL_MODE_ONLY */