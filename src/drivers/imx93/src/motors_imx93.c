/**
 * Motors driver stub for i.MX93 HITL
 */
 #include "motors.h"
 #include "stubs_imx93.h"
 
 static bool isInit = false;
 
 void motorsInit(const MotorPerifDef** motorMapSelect) {
     isInit = true;
 }
 
 bool motorsTest(void) {
     return STUB_RETURN_SUCCESS;
 }
 
 void motorsSetRatio(uint32_t id, uint16_t ratio) {
     // Empty stub - for HITL the motors don't need to do anything
 }
 
 void motorsBeep(int id, bool enable, uint16_t frequency, uint16_t ratio) {
     // Empty stub for beep functionality
 }