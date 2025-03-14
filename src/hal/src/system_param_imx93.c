/**
 * i.MX93-specific system parameter handlers
 */
 #include "param.h"
 #include "fsl_device_registers.h"
 
 /* Static mock values for the flash size and ID if needed */
 static const uint16_t flashSize = 1024; // 1MB flash on i.MX93
 static const uint32_t mcuId[3] = {
   0x4D583933, // "MX93" in hex
   0x00000000, 
   0x00000000
 };
 
 /* Parameter accessor functions */
 uint16_t systemGetFlashSize(void) {
   return flashSize; 
 }
 
 uint32_t systemGetId0(void) {
   return mcuId[0];
 }
 
 uint32_t systemGetId1(void) {
   return mcuId[1];
 }
 
 uint32_t systemGetId2(void) {
   return mcuId[2];
 }