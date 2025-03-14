/**
 * i.MX93 specific platform definitions
 */
 #ifndef MIMX93_H_
 #define MIMX93_H_
 
 #include "fsl_device_registers.h"
 #include "fsl_common.h"
 #include "fsl_rgpio.h"
 
 /* Define equivalents to STM32 types */
 typedef RGPIO_Type* GPIO_TypeDef;
 
 /* MCU specific addresses for parameters - use i.MX93 OCOTP or fixed values */
 #define MCU_ID_ADDRESS          (0x4003208C)  // OCOTP_CFG0 register address
 #define MCU_FLASH_SIZE_ADDRESS  (0x401E4024)  // Flash size register or config area
 
//  /* Define a watchdog reset function */
//  #define watchdogReset()         /* Add i.MX93 watchdog reset code here */
 
 #endif /* MIMX93_H_ */