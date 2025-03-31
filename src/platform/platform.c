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
 * Generic platform functionality
 *
 */

#include <string.h>
#include "platform.h"
#include "fsl_debug_console.h"

static const platformConfig_t* active_config = 0;

int platformInit(void) {
  int nrOfConfigs = 0;
  
  //PRINTF("platformInit: Starting platform initialization\n");
  const platformConfig_t* configs = platformGetListOfConfigurations(&nrOfConfigs);

  //PRINTF("platformInit: Found %d platform configurations\n", nrOfConfigs);
  // Print available configurations
  // for (int i = 0; i < nrOfConfigs; i++) {
  //   PRINTF("platformInit: Available config[%d]: deviceType='%s', name='%s'\n", 
  //         i, configs[i].deviceType, configs[i].deviceTypeName);
  // }
  // // Get current device type
  // char deviceTypeString[PLATFORM_DEVICE_TYPE_STRING_MAX_LEN];
  // platformGetDeviceTypeString(deviceTypeString);
  // PRINTF("platformInit: Device type string from hardware: '%s'\n", deviceTypeString);


  
  int err = platformInitConfiguration(configs, nrOfConfigs);
  //PRINTF("platformInit: platformInitConfiguration returned: %d (0=success, 1=failure)\n", err);
  
  if (err != 0)
  {
    // This firmware is not compatible, abort init
    //PRINTF("platformInit: ERROR - No matching configuration found for this device!\n");
    return 1;
  }

  //PRINTF("platformInit: Configuration matched, initializing hardware...\n");
  platformInitHardware();
  PRINTF("platformInit: Platform initialization complete\n");
  return 0;
}

int platformParseDeviceTypeString(const char* deviceTypeString, char* deviceType) {
  if (deviceTypeString[0] != '0' || deviceTypeString[1] != ';') {
    return 1;
  }

  const int start = 2;
  const int last = start + PLATFORM_DEVICE_TYPE_MAX_LEN - 1;
  int end = 0;
  for (end = start; end <= last; end++) {
    if (deviceTypeString[end] == '\0' || deviceTypeString[end] == ';') {
      break;
    }
  }

  if (end > last) {
    return 1;
  }

  int length = end - start;
  memcpy(deviceType, &deviceTypeString[start], length);
  deviceType[length] = '\0';
  return 0;
}

int platformInitConfiguration(const platformConfig_t* configs, const int nrOfConfigs) {
#ifndef DEVICE_TYPE_STRING_FORCE
  char deviceTypeString[PLATFORM_DEVICE_TYPE_STRING_MAX_LEN];
  char deviceType[PLATFORM_DEVICE_TYPE_MAX_LEN];

  platformGetDeviceTypeString(deviceTypeString);
  int parseResult = platformParseDeviceTypeString(deviceTypeString, deviceType);
  PRINTF("platformInitConfiguration: Raw deviceTypeString='%s', parsed deviceType='%s', parseResult=%d\n", 
         deviceTypeString, deviceType, parseResult);
#else
  #define xstr(s) str(s)
  #define str(s) #s

  char* deviceType = xstr(DEVICE_TYPE_STRING_FORCE);
  PRINTF("platformInitConfiguration: Using forced device type: '%s'\n", deviceType);
#endif

  for (int i = 0; i < nrOfConfigs; i++) {
    const platformConfig_t* config = &configs[i];
    int compareResult = strcmp(config->deviceType, deviceType);
    PRINTF("platformInitConfiguration: Comparing deviceType='%s' with config[%d].deviceType='%s', result=%d (%s)\n", 
           deviceType, i, config->deviceType, compareResult, 
           (compareResult == 0) ? "MATCH" : "NO MATCH");
    
    if (compareResult == 0) {
      active_config = config;
      return 0;
    }
  }

  return 1;
}

const char* platformConfigGetDeviceTypeName() {
  return active_config->deviceTypeName;
}

SensorImplementation_t platformConfigGetSensorImplementation() {
  return active_config->sensorImplementation;
}

bool platformConfigPhysicalLayoutAntennasAreClose() {
  return active_config->physicalLayoutAntennasAreClose;
}
