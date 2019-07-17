#ifndef __ODE_INTEGR_H__
#define __ODE_INTEGR_H__

#include "config_outer.h"
#include "ode_dyn.h"

class TM_val
{

public:
	real tn;
	real curr_tau;
	AF1  *xn; // xn[N_STATE
	OdeFunc *m_f;

	// Temp variables
	#ifdef TIME_STEP_VAR
		AF1 y1_temp[N_STATE];
	#endif
	AF1 y0_temp[N_STATE];

public:
	TM_val(AF1 *x0 , real t0 , OdeFunc *f);
	~TM_val();

	void buildAndEval();

	real fixpoint();

};

#endif // __ODE_INTEGR_H__