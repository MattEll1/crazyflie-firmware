#ifndef OUTER_TASK_H
#define OUTER_TASK_H

/* FreeRtos includes */
extern "C"
{
	#include "FreeRTOS.h"
	#include "task.h"

	#include "log.h"

	#include "console.h"
	#include "cfassert.h"
	#include "debug.h"

	#include "sensors.h"
}

#ifndef STM32F4XX
	#include "ode_integr_emb.h" //#$TODO replace by "ode_integr.h" after having tested SITL
#else
	#include "ode_integr_emb.h"
#endif

//#define Z_LOG_FILE "zlog.txt"

#define INTEG_DURATION  0.11f //0.4
#define RPY_INCERTAINCY 0.0f // Interval(-1.1)
#define PQR_INCERTAINCY	Interval(-0.001 , 0.001) //-0.02 , 0.02
#define UVW_INCERTAINCY	0.0f // Interval(-0.05 , 0.05)
#define Z_INCERTAINCY	Interval(-0.01 , 0.01) //-0.1 , 0.1

void verifTaskInit();

bool verifTaskTest();

void verifTaskLaunch();

#endif //OUTER_TASK_H