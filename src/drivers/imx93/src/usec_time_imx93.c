/**
 * Microsecond timing stub for i.MX93 HITL
 */
 #include "usec_time.h"
 #include "FreeRTOS.h"
 #include "task.h"
 
 uint64_t usecTimestamp(void) {
     // Convert FreeRTOS ticks to microseconds 
     // This isn't precise but good enough for HITL
     return (uint64_t)xTaskGetTickCount() * 1000;
 }