/*
 * FreeRTOS Kernel V10.4.3
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

 #ifndef FREERTOS_CONFIG_H
 #define FREERTOS_CONFIG_H
 
 #include <stdint.h>
 
 /* Include any Crazyflie specific headers */
 #include "config.h"
 #include "cfassert.h"
 
 /* Include board-specific configurations for iMX93 */
 #if defined(__ICCARM__)||defined(__CC_ARM)||defined(__GNUC__)
     /* Clock manager provides in this variable system core clock frequency */
     extern uint32_t SystemCoreClock;
 #endif
 
 /*-----------------------------------------------------------
  * Application specific definitions.
  *
  * These definitions should be adjusted for your particular hardware and
  * application requirements.
  *
  * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
  * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
  *
  * See http://www.freertos.org/a00110.html.
  *----------------------------------------------------------*/
 
 /* ---- Core Configuration ---- */
 #define configUSE_PREEMPTION                    1
 #define configUSE_TICKLESS_IDLE                 0
 #define configCPU_CLOCK_HZ                      (SystemCoreClock)
 #define configTICK_RATE_HZ_RAW                  1000
 #define configTICK_RATE_HZ                      ((TickType_t)configTICK_RATE_HZ_RAW)
 #define configMAX_PRIORITIES                    6  /* Keeping Crazyflie's 6 priorities */
 #define configMINIMAL_STACK_SIZE                ((unsigned short)90)  /* iMX93 value, adjust if needed */
 #define configMAX_TASK_NAME_LEN                 10
 #define configUSE_16_BIT_TICKS                  0
 #define configIDLE_SHOULD_YIELD                 1
 #define configUSE_TASK_NOTIFICATIONS            1
 #define configUSE_MUTEXES                       1
 #define configUSE_RECURSIVE_MUTEXES             1
 #define configUSE_COUNTING_SEMAPHORES           1
 #define configQUEUE_REGISTRY_SIZE               10  /* From Crazyflie for debug purposes */
 #define configUSE_QUEUE_SETS                    0
 #define configUSE_TIME_SLICING                  0
 #define configUSE_NEWLIB_REENTRANT              0
 #define configSTACK_DEPTH_TYPE                  uint32_t
 
 /* ---- Memory Management ---- */
 #define configSUPPORT_STATIC_ALLOCATION         0
 #define configSUPPORT_DYNAMIC_ALLOCATION        1
 #define configTOTAL_HEAP_SIZE                   ((size_t)(FREERTOS_HEAP_SIZE))
 #define configAPPLICATION_ALLOCATED_HEAP        0
 #define configFRTOS_MEMORY_SCHEME               4  /* Using heap_4.c from iMX93 */
 
 /* ---- Hook Functions ---- */
 #ifndef SITL_CF2
     #define configUSE_IDLE_HOOK                 1  /* Crazyflie uses idle hook */
 #else
     #define configUSE_IDLE_HOOK                 0  /* SITL doesn't use watchdog */
 #endif
 #define configUSE_TICK_HOOK                     0
 #ifdef DEBUG
     #define configCHECK_FOR_STACK_OVERFLOW      1  /* Use Crazyflie's debug checks */
 #else
     #define configCHECK_FOR_STACK_OVERFLOW      0
 #endif
 #define configUSE_MALLOC_FAILED_HOOK            1  /* From Crazyflie */
 #define configUSE_DAEMON_TASK_STARTUP_HOOK      0
 
 /* ---- Timer Management ---- */
 #define configUSE_TIMERS                        1
 #define configTIMER_TASK_PRIORITY               1  /* Use Crazyflie's priority */
 #define configTIMER_QUEUE_LENGTH                20 /* Use Crazyflie's length */
 #define configTIMER_TASK_STACK_DEPTH            (configMINIMAL_STACK_SIZE * 4)  /* Crazyflie value */
 
 /* ---- Task Behavior ---- */
 #define configUSE_CO_ROUTINES                   0
 #define configMAX_CO_ROUTINE_PRIORITIES         2
 
 #define INCLUDE_vTaskPrioritySet                1
 #define INCLUDE_uxTaskPriorityGet               1
 #define INCLUDE_vTaskDelete                     1
 #define INCLUDE_vTaskSuspend                    1
 #define INCLUDE_vTaskDelayUntil                 1
 #define INCLUDE_vTaskDelay                      1
 #define INCLUDE_uxTaskGetStackHighWaterMark     1  /* Keep Crazyflie's debug capability */
 #define INCLUDE_xTaskGetIdleTaskHandle          1  /* Keep Crazyflie's requirement */
 #define INCLUDE_xTaskGetCurrentTaskHandle       1
 #define INCLUDE_eTaskGetState                   0
 #define INCLUDE_xResumeFromISR                  1
 #define INCLUDE_xEventGroupSetBitFromISR        1
 #define INCLUDE_xTimerPendFunctionCall          1
 #define INCLUDE_xTaskAbortDelay                 0
 #define INCLUDE_xTaskGetHandle                  0
 #define INCLUDE_xTaskResumeFromISR              1
 
 /* ---- FPU and MPU ---- */
 #ifndef configENABLE_FPU
     #define configENABLE_FPU                    1  /* iMX93 setting */
 #endif
 #ifndef configENABLE_MPU
     #define configENABLE_MPU                    0
 #endif
 #ifndef configENABLE_TRUSTZONE
     #define configENABLE_TRUSTZONE              0
 #endif
 #ifndef configRUN_FREERTOS_SECURE_ONLY
     #define configRUN_FREERTOS_SECURE_ONLY      1
 #endif
 
 /* ---- Debugging ---- */
 #define configUSE_TRACE_FACILITY                1
 #define configGENERATE_RUN_TIME_STATS           0
 #define configUSE_STATS_FORMATTING_FUNCTIONS    0
 #define configRECORD_STACK_HIGH_ADDRESS         1
 #define configINCLUDE_FREERTOS_TASK_C_ADDITIONS_H 1
 
 /* ---- Interrupt Priority Settings ---- */
 #ifdef __NVIC_PRIO_BITS
     /* __NVIC_PRIO_BITS will be specified when CMSIS is being used. */
     #define configPRIO_BITS                     __NVIC_PRIO_BITS
 #else
     #define configPRIO_BITS                     3  /* 8 priority levels */
 #endif
 
 /* The lowest interrupt priority that can be used in a call to a "set priority" function. */
 #define configLIBRARY_LOWEST_INTERRUPT_PRIORITY ((1U << (configPRIO_BITS)) - 1)
 
 /* The highest interrupt priority that can be used by any interrupt service
 routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
 INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
 PRIORITY THAN THIS! (higher priorities are lower numeric values. */
 #define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY 2
 
 /* Interrupt priorities used by the kernel port layer itself.  These are generic
 to all Cortex-M ports, and do not rely on any particular library functions. */
 #define configKERNEL_INTERRUPT_PRIORITY         (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))
 #define configMAX_SYSCALL_INTERRUPT_PRIORITY    (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))
 
 /* ---- Assertion Handling ---- */
 #ifndef SITL_CF2
     #define configASSERT(x)                     if((x) == 0) assertFail(#x, __FILE__, __LINE__)
 #else
     #define configASSERT_BOOL(x)                if((x) == 0) {taskDISABLE_INTERRUPTS(); for (;;);}
     #define configASSERT(x)                     configASSERT_BOOL((x)!=0)
     /* Hack for undefined vApplicationMallocFailedHook */
     extern void vAssertCalled(unsigned long ulLine, const char * const pcFileName);
     extern void vApplicationMallocFailedHook(void);
 #endif
 
 /* ---- Crazyflie Task Tags (For Debugging) ---- */
 #define configUSE_APPLICATION_TASK_TAG          1
 
 #define TASK_LED_ID_NBR                         1
 #define TASK_RADIO_ID_NBR                       2
 #define TASK_STABILIZER_ID_NBR                  3
 #define TASK_ADC_ID_NBR                         4
 #define TASK_PM_ID_NBR                          5
 #define TASK_PROXIMITY_ID_NBR                   6
 
 /* ---- Crazyflie Time Conversion Macros ---- */
 #if configTICK_RATE_HZ_RAW != 1000
     #error "Please review the use of M2T and T2M if there is not a 1 to 1 mapping between ticks and milliseconds"
 #endif
 #define M2T(X)                                  ((unsigned int)(X))
 #define F2T(X)                                  ((unsigned int)((configTICK_RATE_HZ/(X))))
 #define T2M(X)                                  ((unsigned int)(X))
 
 /* ---- Definitions that map the FreeRTOS port interrupt handlers to their CMSIS standard names ---- */
 #define vPortSVCHandler                         SVC_Handler
 #define xPortPendSVHandler                      PendSV_Handler
 #define xPortSysTickHandler                     SysTick_Handler
 
 /* ---- Map Crazyflie-specific handler mappings ---- */
 #define vPortSVCHandler                         SVC_Handler
 #define xPortPendSVHandler                      PendSV_Handler
 #define xPortSysTickHandler                     tickFreeRTOS
 
 /* ---- Crazyflie Queue Monitoring (if enabled) ---- */
 #ifdef DEBUG_QUEUE_MONITOR
     #undef traceQUEUE_SEND
     #undef traceQUEUE_SEND_FAILED
     #define traceQUEUE_SEND(xQueue)             qm_traceQUEUE_SEND(xQueue)
     void qm_traceQUEUE_SEND(void* xQueue);
     #define traceQUEUE_SEND_FAILED(xQueue)      qm_traceQUEUE_SEND_FAILED(xQueue)
     void qm_traceQUEUE_SEND_FAILED(void* xQueue);
 #endif // DEBUG_QUEUE_MONITOR
 
 #endif /* FREERTOS_CONFIG_H */