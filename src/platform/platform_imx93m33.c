
#include <string.h>
#include "platform.h"

// OTP memory configuration for IMX93
#define PLATFORM_INFO_OTP_NR_OF_BLOCKS 16
#define PLATFORM_INFO_OTP_BLOCK_LEN 32

#if PLATFORM_DEVICE_TYPE_STRING_MAX_LEN < (PLATFORM_INFO_OTP_BLOCK_LEN + 1)
#error
#endif

// Default platform string if no valid information is found in OTP
#define DEFAULT_PLATFORM_STRING "0;IMX93"

// On the IMX93, the OTP/OCOTP controller is at a different address
// Consult the IMX93 reference manual for exact addresses
#ifndef UNIT_TEST_MODE
static char* getAddressOfOtpMemoryBlock(int blockNr) {
    // IMX93's OTP memory is accessed differently than STM32
    // You may need to use the OCOTP controller
    // This is a placeholder - you'll need the actual memory map from IMX93 documentation
    return (char*)(0x47510000 + blockNr * 0x20); // Example address - replace with actual
}
#else
// This function is replaced by a mock in unit tests
char* getAddressOfOtpMemoryBlock(const int blockNr);
#endif

// For IMX93, you might need to add additional initialization for OTP access
static void initOtpAccess(void) {
    // Initialize the OCOTP controller
    // This might involve enabling clocks, configuring access timing, etc.
    // Example (placeholder code):
    // *(volatile uint32_t*)(0x47510000) = 0x1; // Enable OTP access
}

void platformGetDeviceTypeString(char* deviceTypeString) {
    // Initialize OTP access if needed
    initOtpAccess();
    
    char* block = 0;
    for (int i = 0; i < PLATFORM_INFO_OTP_NR_OF_BLOCKS; i++) {
        char* candidateBlock = getAddressOfOtpMemoryBlock(i);
        if (candidateBlock[0] != 0) {
            block = candidateBlock;
            break;
        }
    }
    
    if (!block || ((unsigned char)block[0]) == 0xff) {
        block = DEFAULT_PLATFORM_STRING;
    }
    
    strncpy(deviceTypeString, block, PLATFORM_INFO_OTP_BLOCK_LEN);
    deviceTypeString[PLATFORM_INFO_OTP_BLOCK_LEN] = '\0';
}