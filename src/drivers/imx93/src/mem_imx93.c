/**
 * Memory management stubs for i.MX93 HITL
 */
 #include "mem.h"
 #include "eeprom.h"
 #include "stubs_imx93.h"
 #include <string.h>
 
 // Memory subsystem stubs
 void memInit(void) {}
 bool memTest(void) { return STUB_RETURN_SUCCESS; }
 
 // EEPROM stubs
 bool eepromInit(I2C_Dev *i2cPort) {return true;}
 bool eepromTest(void) { return STUB_RETURN_SUCCESS; }
 bool eepromTestConnection(void) { return STUB_RETURN_SUCCESS; }
//  bool eepromWriteBuffer(uint8_t* buffer, uint16_t writeAddr, uint16_t len) {
//      // Fill buffer with zeros to simulate empty EEPROM
//      memset(buffer, 0, len);
//      return STUB_RETURN_SUCCESS;
//  }
 bool eepromWriteBuffer(uint8_t* buffer, uint16_t writeAddr, uint16_t len) {
     return STUB_RETURN_SUCCESS;
 }
 // Make sure this is exactly as expected in the calling code
bool eepromReadBuffer(uint8_t* buffer, uint16_t readAddr, uint16_t len) {
    // Fill buffer with zeros to simulate empty EEPROM
    memset(buffer, 0, len);
    return true;
}