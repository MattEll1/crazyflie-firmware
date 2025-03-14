/**
 * i.MX93 watchdog implementation (simplified for HITL)
 */
 #include "watchdog.h"
 #include "debug.h"
 
 static bool isInit = false;
 
 // Initialize watchdog
 void watchdogInit(void)
 {
   if(isInit)
     return;
 
   DEBUG_PRINT("STUB: i.MX93 watchdog initialized (simulated for HITL)\n");
   isInit = true;
 }
 
 // Test if watchdog initialized correctly
 bool watchdogNormalStartTest(void)
 {
   return isInit;
 }
 
 // Watchdog reset implementation (stub for HITL)
 void IWDG_ReloadCounter(void)
 {
   // Empty stub - for HITL simulation we don't need actual watchdog functionality
   // This function is called through the watchdogReset() macro
 }