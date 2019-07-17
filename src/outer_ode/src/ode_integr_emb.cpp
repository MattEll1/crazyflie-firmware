#include "ode_integr_emb.h"
#include <cstdio>

#include <ctime>

TM_val::TM_val(AF1 *x0 , real t0 , OdeFunc *f)
{
	this->tn = t0;
	this->xn = x0;
    this->m_f = f;
    this->curr_tau = H_MAX;
}

TM_val::~TM_val(){}

void TM_val::buildAndEval()
{   
    uint8_t i;

    // std::cout << "+++" << std::endl;
    // Use fixpoint iteration to get the step size and the remainder
	real new_curr_tau = fixpoint();
	
    // Save f(rough enclosure)
    AF1 frough[N_STATE];
    this->m_f->operator () (frough , this->y0_temp);

	// Add the remainder to the previous sum
    real temp_tau = new_curr_tau; 
	for(i=0 ; i< N_STATE ; i++){
		this->xn[i] += frough[i] * temp_tau;
	}

	// Update the new time
	this->tn += new_curr_tau;
    this->curr_tau = new_curr_tau;

	// Update interval value for xn
	for (i=0 ; i<N_STATE ; i++){
        //this->xn[i].print_af();
        //printf("Rayon : %f\n", this->xn[i].getRadius());
		this->xn[i].compress_af(NOISE_TOL);
        //  this->xn[i].print_af();
		// this->xn_int[i] = this->xn[i].getInterval();
	}

#ifdef TIME_STEP_VAR
    this->curr_tau *= 1.5f;
#endif

}

real TM_val::fixpoint()
{
	// calcul de x satisfaisant x0 + [0,tau][f](x) \subseteq x
    AF1 y1[N_STATE];
    AF1 fx0[N_STATE];
    real step = this->curr_tau;

    uint8_t iter;
    Interval widen, coeff;
    
    this->m_f->operator () (fx0 , this->xn);

    for (uint8_t i=0; i<N_STATE ; i++){
        y1[i] = this->xn[i] + fx0[i].getInterval();
#ifdef TIME_STEP_VAR
        this->y1_temp[i] = y1[i]; 
#endif
    }
    
    iter = 1;
    widen = Interval(-1,1);

    uint8_t found_fixpoint = 0;
    while (iter <=1 || (found_fixpoint == 0))
    {
#ifdef TIME_STEP_VAR
        if (iter % 7 == 0){
            step *= 0.5f;
            iter = 1;
            for(uint8_t i= 0 ; i< N_STATE ; i++)
                y1[i] = this->y1_temp[i];
        }
#endif
        if (iter > 25)
            coeff = 1;
        else if (iter > 20)
            coeff = 0.1;
        else if (iter > 15)
            coeff = 0.01;
        else if (iter > 10)
            coeff = 0.001;
        else if (iter > 5)
            coeff = 0.0001;
        else if (iter > 2)
            coeff = 0.00001;
        
        if (iter > 2)
        {
            for (uint8_t i=0; i<N_STATE ; i++){
                y1[i] = y1[i] + coeff*widen*y1[i].getInterval();
            }
        }

        for(uint8_t i=0 ; i<N_STATE ; i++){
            y1[i].compress_af(NOISE_TOL);
            this->y0_temp[i] = y1[i];
        }

        this->m_f->operator () (fx0 , y1);

        found_fixpoint = 1;
        for (uint8_t i=0; i<N_STATE ; i++){
        	y1[i] = this->xn[i] + Interval(0,step) * fx0[i].getInterval();
        	// y1[i].getInterval().print_it();
            if ( subseteq(y1[i].getInterval() , this->y0_temp[i].getInterval()) == 0){
        		found_fixpoint = 0;
                // i = N_STATE;
        	}
        }
        iter = iter+1;
        // printf("Iter = %d\n", iter);
    }
    // std::cout << 
    // printf("iter = %d , step = %f \n", (int) iter , step);
    return step;
}