/**
 * RAM-only implementation of the configblock for IMX93
 */
 #define DEBUG_MODULE "CFGBLK"

 #include <stdint.h>
 #include <stdbool.h>
 #include <string.h>
 
 #include "config.h"
 #include "debug.h"
 #include "configblock.h"
//  #include "fsl_debug_console.h"
 
 /* Internal format of the config block */
 #define MAGIC 0x43427830
 #define VERSION 1
 
 // Current version structure - kept same as original
 struct configblock_v1_s {
   /* header */
   uint32_t magic;
   uint8_t  version;
   /* Content */
  //  uint8_t radioChannel;
  //  uint8_t radioSpeed;
   float calibPitch;
   float calibRoll;
  //  uint8_t radioAddress_upper;
  //  uint32_t radioAddress_lower;
   /* Simple modulo 256 checksum */
   uint8_t cksum;
 } __attribute__((__packed__));
 
 typedef struct configblock_v1_s configblock_t;
 
 // In-memory config storage
 static configblock_t configblock = {
     .magic = MAGIC,
     .version = VERSION,
    //  .radioChannel = RADIO_CHANNEL,
    //  .radioSpeed = RADIO_DATARATE,
     .calibPitch = 0.0,
     .calibRoll = 0.0,
    //  .radioAddress_upper = ((uint64_t)RADIO_ADDRESS >> 32),
    //  .radioAddress_lower = (RADIO_ADDRESS & 0xFFFFFFFFULL),
     .cksum = 0  // Will be ignored in this implementation
 };
 
 static bool isInit = false;
 
 int configblockInit(void)
 {
   if(isInit)
     return 0;
 
   // Just print that we're using RAM-only config
  //  PRINTF("Using RAM-only configblock for IMX93\n");
   
   isInit = true;
   return 0;
 }
 
 bool configblockTest(void)
 {
   // Always pass the test
  //  PRINTF("configblockTest: RAM implementation [PASS]\n");
   return true;
 }
 
 /* Static accessors - always return from our RAM structure */
//  int configblockGetRadioChannel(void)
//  {
//    return configblock.radioChannel;
//  }
 
//  int configblockGetRadioSpeed(void)
//  {
//    return configblock.radioSpeed;
//  }
 
//  uint64_t configblockGetRadioAddress(void)
//  {
//    return ((uint64_t)configblock.radioAddress_upper << 32) | 
//           (uint64_t)configblock.radioAddress_lower;
//  }
 
 float configblockGetCalibPitch(void)
 {
   return configblock.calibPitch;
 }
 
 float configblockGetCalibRoll(void)
 {
   return configblock.calibRoll;
 }