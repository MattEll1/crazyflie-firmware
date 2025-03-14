/**
 * i.MX93-specific system parameter accessor functions
 */
 #ifndef SYSTEM_PARAM_IMX93_H_
 #define SYSTEM_PARAM_IMX93_H_
 
 #include <stdint.h>
 
 uint16_t systemGetFlashSize(void);
 uint32_t systemGetId0(void);
 uint32_t systemGetId1(void);
 uint32_t systemGetId2(void);
 
 #endif /* SYSTEM_PARAM_IMX93_H_ */