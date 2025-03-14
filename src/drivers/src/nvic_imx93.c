/**
 *    ||          ____  _ __                           
 * +------+      / __ )(_) /_______________ _____  ___ 
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware - i.MX93 Port
 *
 * Contains all Cortex-M33 processor exceptions handlers
 */
 #include "fsl_common.h"
 #include "fsl_debug_console.h"
 #include "led.h"
 #include "cfassert.h"
 
 #define DONT_DISCARD __attribute__((used))
 
 /**
  * Initialize the NVIC (Nested Vectored Interrupt Controller)
  */
 void nvicInit(void)
 {
     // The i.MX93 uses Cortex-M33, which has the same NVIC as M4
     // Set priority grouping - all bits for preemption priority (like STM32's NVIC_PriorityGroup_4)
     NVIC_SetPriorityGrouping(0);
 }
 
 /**
  * @brief  This function handles SysTick Handler.
  */
 extern void tickFreeRTOS(void);
 
//  void DONT_DISCARD SysTick_Handler(void)
//  {
//      // Call FreeRTOS tick handler - same as original code
//      tickFreeRTOS();
//  }
 
 /**
   * @brief  This function handles NMI exception.
   */
 void DONT_DISCARD NMI_Handler(void)
 {
     // Same implementation as original
 }
 
 /**
  * @brief  This function handles Hard Fault exception.
  */
 void DONT_DISCARD HardFault_Handler(void)
 {
     // Same implementation as original
     asm("TST LR, #4");
     asm("ITE EQ");
     asm("MRSEQ R0, MSP");
     asm("MRSNE R0, PSP");
     asm("B printHardFault");
 }
 
 void DONT_DISCARD printHardFault(uint32_t* hardfaultArgs)
 {
     unsigned int stacked_r0 = ((unsigned long) hardfaultArgs[0]);
     unsigned int stacked_r1 = ((unsigned long) hardfaultArgs[1]);
     unsigned int stacked_r2 = ((unsigned long) hardfaultArgs[2]);
     unsigned int stacked_r3 = ((unsigned long) hardfaultArgs[3]);
     unsigned int stacked_r12 = ((unsigned long) hardfaultArgs[4]);
     unsigned int stacked_lr = ((unsigned long) hardfaultArgs[5]);
     unsigned int stacked_pc = ((unsigned long) hardfaultArgs[6]);
     unsigned int stacked_psr = ((unsigned long) hardfaultArgs[7]);
 
     // Use NXP's debug console instead of UART_PRINT
     PRINTF("[Hard fault handler]\r\n");
     PRINTF("R0 = %x\r\n", stacked_r0);
     PRINTF("R1 = %x\r\n", stacked_r1);
     PRINTF("R2 = %x\r\n", stacked_r2);
     PRINTF("R3 = %x\r\n", stacked_r3);
     PRINTF("R12 = %x\r\n", stacked_r12);
     PRINTF("LR = %x\r\n", stacked_lr);
     PRINTF("PC = %x\r\n", stacked_pc);
     PRINTF("PSR = %x\r\n", stacked_psr);
     
     // These register addresses are the same for Cortex-M33
     PRINTF("BFAR = %x\r\n", (*((volatile unsigned int *)(0xE000ED38))));
     PRINTF("CFSR = %x\r\n", (*((volatile unsigned int *)(0xE000ED28))));
     PRINTF("HFSR = %x\r\n", (*((volatile unsigned int *)(0xE000ED2C))));
     PRINTF("DFSR = %x\r\n", (*((volatile unsigned int *)(0xE000ED30))));
     PRINTF("AFSR = %x\r\n", (*((volatile unsigned int *)(0xE000ED3C))));
 
     // For HITL, we don't need to actually stop motors - they're simulated
     // But we still set error LEDs if available
     ledClearAll();
     ledSet(ERR_LED1, 1);
     ledSet(ERR_LED2, 1);
 
     storeAssertSnapshotData(__FILE__, __LINE__);
     while (1) {}
 }
 
 /**
  * @brief  This function handles Memory Manage exception.
  */
 void DONT_DISCARD MemManage_Handler(void)
 {
     // Simplified for HITL
     ledClearAll();
     ledSet(ERR_LED1, 1);
     ledSet(ERR_LED2, 1);
 
     storeAssertSnapshotData(__FILE__, __LINE__);
     while (1) {}
 }
 
 /**
  * @brief  This function handles Bus Fault exception.
  */
 void DONT_DISCARD BusFault_Handler(void)
 {
     // Simplified for HITL
     ledClearAll();
     ledSet(ERR_LED1, 1);
     ledSet(ERR_LED2, 1);
 
     storeAssertSnapshotData(__FILE__, __LINE__);
     while (1) {}
 }
 
 /**
  * @brief  This function handles Usage Fault exception.
  */
 void DONT_DISCARD UsageFault_Handler(void)
 {
     // Simplified for HITL
     ledClearAll();
     ledSet(ERR_LED1, 1);
     ledSet(ERR_LED2, 1);
 
     storeAssertSnapshotData(__FILE__, __LINE__);
     while (1) {}
 }
 
 /**
  * @brief  This function handles Debug Monitor exception.
  */
 void DONT_DISCARD DebugMon_Handler(void)
 {
     // Same as original - empty
 }
 
 /**
  * @brief  This function handles Secure Fault exception (M33 specific).
  */
 void DONT_DISCARD SecureFault_Handler(void)
 {
     // New handler for Cortex-M33
     PRINTF("[Secure fault handler]\r\n");
     PRINTF("SFSR = %x\r\n", (*((volatile unsigned int *)(0xE000EDE4))));
     PRINTF("SFAR = %x\r\n", (*((volatile unsigned int *)(0xE000EDE8))));
     
     ledClearAll();
     ledSet(ERR_LED1, 1);
     ledSet(ERR_LED2, 1);
 
     storeAssertSnapshotData(__FILE__, __LINE__);
     while (1) {}
 }
 
 // Note: We don't need the DMA handler for WS2812 in HITL mode,
 // but if your port uses it, you would implement it with the appropriate DMA IRQ