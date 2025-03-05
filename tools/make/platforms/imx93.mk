# Make configuration for the UCM-iXM93 platform

PLATFORM_HELP_hitl = iMX93 HITL platform for Simulation
PLATFORM_NAME_hitl = iMX93 Simulation platform

CPU=imx93m33

######### Sensors configuration ##########
CFLAGS += -DSENSOR_INCLUDED_SIM
PROJ_OBJ += sensors_sim.o

######### Stabilizer configuration ##########
ESTIMATOR          ?= any
CONTROLLER         ?= Any # one of Any, PID, Mellinger
POWER_DISTRIBUTION ?= sim

# CFLAGS += -DDEBUG=1
# CFLAGS += -DDEBUG_PRINT_ON_UART
# CFLAGS += -DDEBUG_MOTORS
# CFLAGS += -DDEBUG_QUEUE_MONITOR
