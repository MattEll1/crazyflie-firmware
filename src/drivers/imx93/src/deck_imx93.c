/**
 * Deck and peripheral stubs for i.MX93 HITL
 */
 #include "deck.h"
 #include "deck_core.h"
 #include "i2cdev.h"
 #include "i2c_drv.h"
 #include "stubs_imx93.h"
 
 // Global variable needed by configblock
 I2cDrv deckBus = {0};
 // Deck stubs
 void deckInit(void) {}
 bool deckTest(void) { return STUB_RETURN_SUCCESS; }
 StateEstimatorType deckGetRequiredEstimator(void) { return anyEstimator; }
 bool deckGetRequiredLowInterferenceRadioMode(void) { return false; }
 
 // I2C device stubs
 int i2cdevInit(I2C_Dev *dev) {return 0;}
 bool i2cdevTest(void) { return STUB_RETURN_SUCCESS; }