/**
 *    ||          ____  _ __                           
 * +------+      / __ )(_) /_______________ _____  ___ 
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware
 *
 * Copyright (C) 2011-2012 Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * main.c - Containing the main function.
 */

/* Personal configs */
#include "FreeRTOSConfig.h"

/* FreeRtos includes */
#include "FreeRTOS.h"
#include "task.h"

/* Project includes */
#include "config.h"
#include "platform.h"
#include "system.h"
#include "usec_time.h"

#include "led.h"
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_cache.h"

/* ST includes */
//#include "stm32fxxx.h"

void ConfigureCachesForDDR(void)
{
    /* Disable caches using the XCACHE API */
    XCACHE_DisableCache(LPCAC_PC);
    
    /* Configure MPU for DDR region */
    ARM_MPU_Disable();
    
    /* Configure the region for DDR memory (assuming region 0) */
    /* Using the ARMv8-M MPU API which requires RNR, RBAR, and RLAR parameters */
    uint8_t region_num = 0;
    
    /* Region base address */
    uint32_t rbar = 0x90000000U | /* Base address */
                   (0U << MPU_RBAR_SH_Pos) | /* Non-shareable */
                   (0U << MPU_RBAR_AP_Pos) | /* Full access permissions */
                   (0U << MPU_RBAR_XN_Pos);  /* Allow execution */
    
    /* Region limit address - 64MB region */
    uint32_t rlar = (0x90000000U + (64 * 1024 * 1024) - 1) | /* Set limit address for 64MB */
                   (1U << MPU_RLAR_EN_Pos) | /* Enable region */
                   (0U << MPU_RLAR_AttrIndx_Pos); /* Use attribute index 0 */
    
    /* Set up memory attributes in MAIR */
    ARM_MPU_SetMemAttr(0, ARM_MPU_ATTR(
        ARM_MPU_ATTR_MEMORY_(1, 1, 1, 0), /* Normal memory, inner write-back */
        ARM_MPU_ATTR_MEMORY_(1, 1, 1, 0)  /* Normal memory, outer write-back */
    ));
    
    /* Set the region */
    ARM_MPU_SetRegion(region_num, rbar, rlar);
    
    /* Enable MPU with default memory map as background */
    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk);
    
    /* Memory barriers */
    __DSB();
    __ISB();
    
    /* Re-enable caches */
    XCACHE_EnableCache(LPCAC_PC);
}


int main() 
{
  BOARD_InitHardware();
  PRINTF("main: Debug console initialized\n");
  PRINTF("main: UCM-iMX93 Crazyflie Firmware Starting\n");

  //Initialize the platform.
  int err = platformInit();
  PRINTF("main: Platform initialization %s (error code: %d)\n", err == 0 ? "successful" : "failed", err);

  if (err != 0) {
    // The firmware is running on the wrong hardware. Halt
    while(1);
  }

  PRINTF("main: Launching system\n");
  //Launch the system task that will initialize and start everything
  systemLaunch();
  PRINTF("main: Launching system sucessful!\n");

  PRINTF("main: Starting FreeRTOS schedueler...\n");
  //Start the FreeRTOS scheduler
  vTaskStartScheduler();

  PRINTF("main: Schedueler failed! Should not reach this point\n");
  // //TODO: Move to platform launch failed
  // ledInit();
  // ledSet(0, 1);
  // ledSet(1, 1);

  //Should never reach this point!
  while(1);

  return 0;

}

