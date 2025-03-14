/**
 * i.MX93 configuration and compatibility definitions
 */
 #ifndef IMX93_CONF_H
 #define IMX93_CONF_H
 
 #include "cfassert.h"
 
 /* Define STM32 compatibility macros */
 #ifdef DEBUG
   #define assert_param(expr) ASSERT(expr)
 #else
   #define assert_param(expr) ((void)0)
 #endif
 
 #endif /* IMX93_CONF_H */