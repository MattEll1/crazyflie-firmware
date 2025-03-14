/**
 * Power management stub for i.MX93 HITL
 */
 #include "pm.h"
 #include "stubs_imx93.h"
 
 static bool isInit = false;
 
 void pmInit(void) {
     isInit = true;
 }
 
 bool pmTest(void) {
     return STUB_RETURN_SUCCESS;
 }
 
 float pmGetBatteryVoltage(void) {
     return STUB_BATTERY_VOLTAGE;
 }