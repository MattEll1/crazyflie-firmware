/**
 * LED driver stub for i.MX93 HITL
 */
 #include "led.h"
 #include "stubs_imx93.h"
 #include "fsl_debug_console.h"
 
 static bool isInit = false;
 
 void ledInit(void) {
     isInit = true;
     PRINTF("LED stub initialized\r\n");
 }
 
 bool ledTest(void) {
     return STUB_RETURN_SUCCESS;
 }
 
 void ledSet(led_t led, bool value) {
     // Optionally log LED state changes
     // PRINTF("LED %d set to %d\r\n", led, value);
 }
 
 void ledClearAll(void) {
     // PRINTF("All LEDs cleared\r\n");
 }