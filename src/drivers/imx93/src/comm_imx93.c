/**
 * Communication stubs for i.MX93 HITL
 */
 #include <stddef.h>
 #include "radiolink.h"
 #include "syslink.h"
 #include "stubs_imx93.h"
 
 // Radio link stubs
 void radiolinkInit(void) {}
 bool radiolinkTest(void) { return STUB_RETURN_SUCCESS; }
 struct crtpLinkOperations* radiolinkGetLink(void) { return NULL; }
 void radiolinkSetPowerDbm(int8_t powerDbm) {}
 
 // UART link stubs
 void uartslkInit(void) {}
 bool uartslkTest(void) { return STUB_RETURN_SUCCESS; }
 
 // Syslink stubs
 void syslinkInit(void) {}
 bool syslinkTest(void) { return STUB_RETURN_SUCCESS; }
 int syslinkSendPacket(SyslinkPacket *slp) { return STUB_RETURN_SUCCESS; }
 
 // Additional communication stubs
 void lpsSendLppShort(uint8_t destId, void* data, size_t length) {}
 bool outlierFilterVaildateTdoaSteps(uint8_t anchorIdA, uint8_t anchorIdB, float tdoa) { 
     return STUB_RETURN_SUCCESS; 
 }