/**
 *    ||          ____  _ __                           
 * +------+      / __ )(_) /_______________ _____  ___ 
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware
 *
 * Copyright (C) 2011-2018 Bitcraze AB
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
 * Platform functionality for the IMX93 HITL platform
 */

#define DEBUG_MODULE "PLATFORM"

#include <string.h>

#include "platform.h"
#include "exti_imx93.h"
#include "nvic_imx93.h"
#include "debug.h"
#include "board.h"
#include "fsl_debug_console.h"

static platformConfig_t configs[] = {
  {
    .deviceType = "IMX93",
    .deviceTypeName = "UCM-IMX93",
    .sensorImplementation = SensorImplementation_sim,
    .physicalLayoutAntennasAreClose = true,
  }
  // {
  //   .deviceType = "CF21",
  //   .deviceTypeName = "Crazyflie 2.1",
  //   .sensorImplementation = SensorImplementation_sim,
  //   .physicalLayoutAntennasAreClose = true,
  // }
};

const platformConfig_t* platformGetListOfConfigurations(int* nrOfConfigs) {
  *nrOfConfigs = sizeof(configs) / sizeof(platformConfig_t);
  return configs;
}

void platformInitHardware() {
  // PRINTF("Starting iMX93 hardware initialization...\n");
  // BOARD_InitDebugConsole();
  // PRINTF("Debug console initialized\n");

  //Low level init: Clock and Interrupt controller
  PRINTF("Initializing NVIC...\n");
  nvicInit();
  PRINTF("NVIC initialized\n");

  //EXTI interrupts
  PRINTF("Initializing EXTI...\n");
  extiInit();
  PRINTF("EXTI initialized\n");

  //DEBUG_PRINT("iMX93 platform initialized\n");
  PRINTF("iMX93 platform initialized\n");
}


// Config functions ------------------------

const char* platformConfigGetPlatformName() {
  return "imx93";
}

