/**
 *    ||          ____  _ __
 * +------+      / __ )(_) /_______________ _____  ___
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware - i.MX93 Port
 *
 * GPIO interrupt implementation for i.MX93 (replacing EXTI functionality)
 */
 #include <stdbool.h>

 #include "fsl_rgpio.h"
 #include "fsl_common.h"
 #include "fsl_clock.h"
 
 // Priority definitions - could be moved to a header
 #define GPIO_IRQ_PRIORITY     5   // Medium priority
 
 static bool isInit;
 
 // Keep the callback structure the same as the original crazyflie
 void __attribute__((weak)) EXTI0_Callback(void) { }
 void __attribute__((weak)) EXTI1_Callback(void) { }
 void __attribute__((weak)) EXTI2_Callback(void) { }
 void __attribute__((weak)) EXTI3_Callback(void) { }
 void __attribute__((weak)) EXTI4_Callback(void) { }
 void __attribute__((weak)) EXTI5_Callback(void) { }
 void __attribute__((weak)) EXTI6_Callback(void) { }
 void __attribute__((weak)) EXTI7_Callback(void) { }
 void __attribute__((weak)) EXTI8_Callback(void) { }
 void __attribute__((weak)) EXTI9_Callback(void) { }
 void __attribute__((weak)) EXTI10_Callback(void) { }
 void __attribute__((weak)) EXTI11_Callback(void) { }
 void __attribute__((weak)) EXTI12_Callback(void) { }
 void __attribute__((weak)) EXTI13_Callback(void) { }
 void __attribute__((weak)) EXTI14_Callback(void) { }
 void __attribute__((weak)) EXTI15_Callback(void) { }
 
 /**
  * Initialize the GPIO interrupt functionality
  * This replaces extiInit() from the original crazyflie firmware
  */
 void extiInit(void)
 {
     if (isInit) {
         return;
     }
 
     // Enable clocks for RGPIO peripherals
     CLOCK_EnableClock(kCLOCK_Rgpio1);
     CLOCK_EnableClock(kCLOCK_Rgpio2);
 
     // Configure NVIC for GPIO interrupts - 
     // These are the actual IRQ numbers for i.MX93 (check startup_MIMX9352_cm33.S)
     // Note: GPIO1 and GPIO2 interrupts are available as Reserved26_IRQHandler etc.
     // in the vector table, but should be accessed through SDK-defined IRQ numbers
     
     // Set priority for all GPIO interrupts
     NVIC_SetPriority(GPIO1_Combined_0_15_IRQn, GPIO_IRQ_PRIORITY);
     NVIC_SetPriority(GPIO1_Combined_16_31_IRQn, GPIO_IRQ_PRIORITY);
     
     // Enable the interrupts in NVIC
     EnableIRQ(GPIO1_Combined_0_15_IRQn);
     EnableIRQ(GPIO1_Combined_16_31_IRQn);
     
     // Note: In HITL mode you might not need GPIO2, 
     // but include if needed for your specific implementation
     NVIC_SetPriority(GPIO2_Combined_0_15_IRQn, GPIO_IRQ_PRIORITY);
     NVIC_SetPriority(GPIO2_Combined_16_31_IRQn, GPIO_IRQ_PRIORITY);
     EnableIRQ(GPIO2_Combined_0_15_IRQn);
     EnableIRQ(GPIO2_Combined_16_31_IRQn);
     
     isInit = true;
 }
 
 /**
  * Test if the EXTI functionality is initialized
  */
 bool extiTest(void)
 {
     return isInit;
 }
 
 /**
  * Configure a GPIO pin for interrupt generation
  * This is a helper function
  */
  static void extiConfigureLine(GPIO_Type *base, uint32_t pin, rgpio_interrupt_mode_t mode)
  {
      // Enable interrupt for the specified pin
      RGPIO_SetPinInterruptConfig(base, pin, mode);
      RGPIO_EnableInterrupt(base, (1U << pin));
  }
 
 /**
  * GPIO1 pins 0-15 combined interrupt handler
  */
 void GPIO1_Combined_0_15_IRQHandler(void)
 {
     // Get flags to determine which pins triggered the interrupt
     uint32_t intFlags = RGPIO_GetPinsInterruptFlags(GPIO1);
     
     // Handle each pin individually - translate to the original EXTI structure
     if (intFlags & (1U << 0)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 0));
         EXTI0_Callback();
     }
     
     if (intFlags & (1U << 1)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 1));
         EXTI1_Callback();
     }
     
     if (intFlags & (1U << 2)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 2));
         EXTI2_Callback();
     }
     
     if (intFlags & (1U << 3)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 3));
         EXTI3_Callback();
     }
     
     if (intFlags & (1U << 4)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 4));
         EXTI4_Callback();
     }
     
     if (intFlags & (1U << 5)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 5));
         EXTI5_Callback();
     }
     
     if (intFlags & (1U << 6)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 6));
         EXTI6_Callback();
     }
     
     if (intFlags & (1U << 7)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 7));
         EXTI7_Callback();
     }
     
     if (intFlags & (1U << 8)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 8));
         EXTI8_Callback();
     }
     
     if (intFlags & (1U << 9)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 9));
         EXTI9_Callback();
     }
     
     if (intFlags & (1U << 10)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 10));
         EXTI10_Callback();
     }
     
     if (intFlags & (1U << 11)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 11));
         EXTI11_Callback();
     }
     
     if (intFlags & (1U << 12)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 12));
         EXTI12_Callback();
     }
     
     if (intFlags & (1U << 13)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 13));
         EXTI13_Callback();
     }
     
     if (intFlags & (1U << 14)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 14));
         EXTI14_Callback();
     }
     
     if (intFlags & (1U << 15)) {
         RGPIO_ClearPinsInterruptFlags(GPIO1, (1U << 15));
         EXTI15_Callback();
     }
 }
 
 /**
  * GPIO1 pins 16-31 combined interrupt handler
  * In HITL mode, you might not need this
  */
 void GPIO1_Combined_16_31_IRQHandler(void)
 {
     // Get flags to determine which pins triggered the interrupt
     uint32_t intFlags = RGPIO_GetPinsInterruptFlags(GPIO1) >> 16;
     
     // Clear all flags 
     RGPIO_ClearPinsInterruptFlags(GPIO1, (intFlags << 16));
     
     // You could implement additional callbacks for pins 16-31 if needed
 }
 
