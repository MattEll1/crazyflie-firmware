# imx93_obj.mk - Selection of the i.MX93 SDK objects to compile
# This file is part of the Crazy Flie control program ported to i.MX93

# Add path to i.MX93 drivers
VPATH+=$(LIB)/imx93/drivers/src/

# Core drivers
IMX93_OBJ+=fsl_common.o
IMX93_OBJ+=fsl_common_arm.o
IMX93_OBJ+=fsl_clock.o

# Peripheral drivers
IMX93_OBJ+=fsl_rgpio.o          # GPIO replacement
IMX93_OBJ+=fsl_lpuart.o         # UART replacement
IMX93_OBJ+=fsl_lpi2c.o          # I2C replacement
IMX93_OBJ+=fsl_lpspi.o          # SPI replacement
IMX93_OBJ+=fsl_mu.o             # Message Unit for RPMsg
IMX93_OBJ+=fsl_cache.o          # Cache control
#IMX93_OBJ+=fsl_tpm.o            # Timer/PWM Module (for motors)
#IMX93_OBJ+=fsl_sema42.o         # Semaphore (for multi-core communication)

# Debug utilities - needed by some drivers
IMX93_OBJ+=fsl_assert.o
