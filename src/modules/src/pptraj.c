/*
 *    ______
 *   / ____/________ _____  __  ________      ______ __________ ___
 *  / /   / ___/ __ `/_  / / / / / ___/ | /| / / __ `/ ___/ __ `__ \
 * / /___/ /  / /_/ / / /_/ /_/ (__  )| |/ |/ / /_/ / /  / / / / / /
 * \____/_/   \__,_/ /___/\__, /____/ |__/|__/\__,_/_/  /_/ /_/ /_/
 *                       /____/
 *
 * Crazyswarm advanced control firmware for Crazyflie
 *

The MIT License (MIT)

Copyright (c) 2018 Wolfgang Hoenig and James Alan Preiss

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/*
implementation of piecewise polynomial trajectories
See Daniel Mellinger, Vijay Kumar: "Minimum snap trajectory generation and control for quadrotors". ICRA 2011: 2520-2525
*/

#include "pptraj.h"
#include <string.h>

#define GRAV (9.81f)
#define POLY5_COEFFS 6
#define POLY7_COEFFS 8 
#define POLY_LINEAR_COEFFS 2

static struct poly4d poly4d_tmp;

// polynomials are stored with ascending degree

void polylinear(float p[PP_SIZE], float duration, float x0, float x1)
{
	p[0] = x0;
	p[1] = (x1 - x0) / duration;
	for (int i = 2; i < PP_SIZE; ++i) {
		p[i] = 0;
	}
}

void polyscale(float p[PP_SIZE], float s)
{
	for (int i = 0; i < PP_SIZE; ++i) {
		p[i] *= s;
	}
}

// e.g. if s==2 the new polynomial will be stretched to take 2x longer
void polystretchtime(float p[PP_SIZE], float s)
{
	float recip = 1.0f / s;
	float scale = recip;
	for (int i = 1; i < PP_SIZE; ++i) {
		p[i] *= scale;
		scale *= recip;
	}
}

void polyreflect(float p[PP_SIZE])
{
	for (int i = 1; i < PP_SIZE; i += 2) {
		p[i] = -p[i];
	}
}

// evaluate a polynomial using horner's rule.
float polyval(float const p[PP_SIZE], float t)
{
	float x = 0.0;
	for (int i = PP_DEGREE; i >= 0; --i) {
		x = x * t + p[i];
	}
	return x;
}

// compute derivative of a polynomial in place
void polyder(float p[PP_SIZE])
{
	for (int i = 1; i <= PP_DEGREE; ++i) {
		p[i-1] = i * p[i];
	}
	p[PP_DEGREE] = 0;
}

void poly5(float poly[PP_SIZE], float T,
	float x0, float dx0, float ddx0,
	float xf, float dxf, float ddxf)
{
	float T2 = T * T;
	float T3 = T2 * T;
	float T4 = T3 * T;
	float T5 = T4 * T;
	poly[0] = x0;
	poly[1] = dx0;
	poly[2] = ddx0 / 2;
	poly[3] = (-12*dx0*T - 8*dxf*T - 3*ddx0*T2 + ddxf*T2 - 20*x0 + 20*xf)/(2*T3);
	poly[4] = (16*dx0*T + 14*dxf*T + 3*ddx0*T2 - 2*ddxf*T2 + 30*x0 - 30*xf)/(2*T4);
	poly[5] = (-6*dx0*T - 6*dxf*T - ddx0*T2 + ddxf*T2 - 12*x0 + 12*xf)/(2*T5);
	for (int i = 6; i < PP_SIZE; ++i) {
		poly[i] = 0;
	}
};

static void poly7_nojerk(float poly[PP_SIZE], float T,
	float x0, float dx0, float ddx0,
	float xf, float dxf, float ddxf)
{
	float T2 = T * T;
	float T3 = T2 * T;
	float T4 = T3 * T;
	float T5 = T4 * T;
	float T6 = T5 * T;
	float T7 = T6 * T;
	poly[0] = x0;
	poly[1] = dx0;
	poly[2] = ddx0/2;
	poly[3] = 0;
	poly[4] = -(5*(14*x0 - 14*xf + 8*T*dx0 + 6*T*dxf + 2*T2*ddx0 - T2*ddxf))/(2*T4);
	poly[5] = (84*x0 - 84*xf + 45*T*dx0 + 39*T*dxf + 10*T2*ddx0 - 7*T2*ddxf)/T5;
	poly[6] = -(140*x0 - 140*xf + 72*T*dx0 + 68*T*dxf + 15*T2*ddx0 - 13*T2*ddxf)/(2*T6);
	poly[7] = (2*(10*x0 - 10*xf + 5*T*dx0 + 5*T*dxf + T2*ddx0 - T2*ddxf))/T7;
	for (int i = 8; i < PP_SIZE; ++i) {
		poly[i] = 0;
	}
}


//
// 4d single-piece polynomials
//

// construct a 4d zero polynomial.
struct poly4d poly4d_zero(float duration)
{
	struct poly4d p = {
		.p = {{0}},
		.duration = duration,
	};
	return p;
}

struct poly4d poly4d_linear(float duration, struct vec p0, struct vec p1, float yaw0, float yaw1)
{
	// struct poly4d p;
	// p.duration = duration;
	// float aligned_p[4][POLY_LINEAR_COEFFS];

	// polylinear(aligned_p[0], duration, p0.x, p1.x);
	// polylinear(aligned_p[1], duration, p0.y, p1.y);
	// polylinear(aligned_p[2], duration, p0.z, p1.z);
	// polylinear(aligned_p[3], duration, yaw0, yaw1);
	// memcpy(p.p, aligned_p, sizeof(aligned_p));
	// return p;
	struct poly4d p;
    p.duration = duration;
    
    // Initialize all elements to zero
    memset(p.p, 0, sizeof(p.p));
    
    // Set only the values we need
    p.p[0][0] = p0.x;
    p.p[0][1] = (p1.x - p0.x) / duration;
    
    p.p[1][0] = p0.y;
    p.p[1][1] = (p1.y - p0.y) / duration;
    
    p.p[2][0] = p0.z;
    p.p[2][1] = (p1.z - p0.z) / duration;
    
    p.p[3][0] = yaw0;
    p.p[3][1] = (yaw1 - yaw0) / duration;
    
    return p;
}

void poly4d_scale(struct poly4d *p, float x, float y, float z, float yaw)
{
	float* aligned_p[4];
	memcpy(aligned_p, p->p, sizeof(aligned_p));
	polyscale(aligned_p[0], x);
    polyscale(aligned_p[1], y);
    polyscale(aligned_p[2], z);
    polyscale(aligned_p[3], yaw);
}

void poly4d_shift(struct poly4d *p, float x, float y, float z, float yaw)
{
	p->p[0][0] += x;
	p->p[1][0] += y;
	p->p[2][0] += z;
	p->p[3][0] += yaw;
}

void poly4d_stretchtime(struct poly4d *p, float s)
{
	float* aligned_p[4]; 
	memcpy(aligned_p, p->p, sizeof(aligned_p));

	for (int i = 0; i < 4; ++i) {
		polystretchtime(aligned_p[i], s);
	}
	p->duration *= s;
}

void polyder4d(struct poly4d *p)
{
	float* aligned_p[4];
	memcpy(aligned_p, p->p, sizeof(aligned_p));

	for (int i = 0; i < 4; ++i) {
		polyder(aligned_p[i]);
	}
}

static struct vec polyval_xyz(struct poly4d const *p, float t)
{
	float* aligned_p[4];
	memcpy(aligned_p, p->p, sizeof(aligned_p));
	return mkvec(polyval(aligned_p[0], t), polyval(aligned_p[1], t), polyval(aligned_p[2], t));
}

static float polyval_yaw(struct poly4d const *p, float t)
{
	float* aligned_p[4];
	memcpy(aligned_p, p->p, sizeof(aligned_p));
	return polyval(aligned_p[3], t);
}

// compute loose maximum of acceleration -
// uses L1 norm instead of Euclidean, evaluates polynomial instead of root-finding
float poly4d_max_accel_approx(struct poly4d const *p)
{
	struct poly4d* acc = &poly4d_tmp;
	*acc = *p;
	polyder4d(acc);
	polyder4d(acc);
	int steps = 10 * p->duration;
	float step = p->duration / (steps - 1);
	float t = 0;
	float amax = 0;
	for (int i = 0; i < steps; ++i) {
		struct vec ddx = polyval_xyz(acc, t);
		float ddx_minkowski = vminkowski(ddx);
		if (ddx_minkowski > amax) amax = ddx_minkowski;
		t += step;
	}
	return amax;
}

struct traj_eval traj_eval_invalid()
{
	struct traj_eval ev;
	ev.pos = vrepeat(NAN);
	return ev;
}

bool is_traj_eval_valid(struct traj_eval const *ev)
{
	return !visnan(ev->pos);
}

struct traj_eval poly4d_eval(struct poly4d const *p, float t)
{
	// flat variables
	struct traj_eval out;
	out.pos = polyval_xyz(p, t);
	out.yaw = polyval_yaw(p, t);

	// 1st derivative
	struct poly4d* deriv = &poly4d_tmp;
	*deriv = *p;
	polyder4d(deriv);
	out.vel = polyval_xyz(deriv, t);
	float dyaw = polyval_yaw(deriv, t);

	// 2nd derivative
	polyder4d(deriv);
	out.acc = polyval_xyz(deriv, t);

	// 3rd derivative
	polyder4d(deriv);
	struct vec jerk = polyval_xyz(deriv, t);

	struct vec thrust = vadd(out.acc, mkvec(0, 0, GRAV));
	// float thrust_mag = mass * vmag(thrust);

	struct vec z_body = vnormalize(thrust);
	struct vec x_world = mkvec(cos(out.yaw), sin(out.yaw), 0);
	struct vec y_body = vnormalize(vcross(z_body, x_world));
	struct vec x_body = vcross(y_body, z_body);

	struct vec jerk_orth_zbody = vorthunit(jerk, z_body);
	struct vec h_w = vscl(1.0f / vmag(thrust), jerk_orth_zbody);

	out.omega.x = -vdot(h_w, y_body);
	out.omega.y = vdot(h_w, x_body);
	out.omega.z = z_body.z * dyaw;

	return out;
}

//
// piecewise 4d polynomials
//

// piecewise eval
struct traj_eval piecewise_eval(
  struct piecewise_traj const *traj, float t)
{
	int cursor = 0;
	t = t - traj->t_begin;
	while (cursor < traj->n_pieces) {
		struct poly4d const *piece = &(traj->pieces[cursor]);
		if (t <= piece->duration * traj->timescale) {
			poly4d_tmp = *piece;
			poly4d_shift(&poly4d_tmp, traj->shift.x, traj->shift.y, traj->shift.z, 0);
			poly4d_stretchtime(&poly4d_tmp, traj->timescale);
			return poly4d_eval(&poly4d_tmp, t);
		}
		t -= piece->duration * traj->timescale;
		++cursor;
	}
	// if we get here, the trajectory has ended
	struct poly4d const *end_piece = &(traj->pieces[traj->n_pieces - 1]);
	struct traj_eval ev = poly4d_eval(end_piece, end_piece->duration);
	ev.pos = vadd(ev.pos, traj->shift);
	ev.vel = vzero();
	ev.acc = vzero();
	ev.omega = vzero();
	return ev;
}

struct traj_eval piecewise_eval_reversed(
  struct piecewise_traj const *traj, float t)
{
	int cursor = traj->n_pieces - 1;
	t = t - traj->t_begin;
	while (cursor >= 0) {
		struct poly4d const *piece = &(traj->pieces[cursor]);
		if (t <= piece->duration * traj->timescale) {
			poly4d_tmp = *piece;
			poly4d_shift(&poly4d_tmp, traj->shift.x, traj->shift.y, traj->shift.z, 0);
			poly4d_stretchtime(&poly4d_tmp, traj->timescale);
			for (int i = 0; i < 4; ++i) {
				// Only negate the first few elements that actually exist
				for (int j = 1; j < 4 && j < PP_SIZE; j += 2) {
					poly4d_tmp.p[i][j] = -poly4d_tmp.p[i][j];
				}
			}
			t = t - piece->duration * traj->timescale;
			return poly4d_eval(&poly4d_tmp, t);
		}
		t -= piece->duration * traj->timescale;
		--cursor;
	}
	// if we get here, the trajectory has ended
	struct poly4d const *end_piece = &(traj->pieces[0]);
	struct traj_eval ev = poly4d_eval(end_piece, 0.0f);
	ev.pos = vadd(ev.pos, traj->shift);
	ev.vel = vzero();
	ev.acc = vzero();
	ev.omega = vzero();
	return ev;
}

// y, dy == yaw, derivative of yaw
void piecewise_plan_5th_order(struct piecewise_traj *pp, float duration,
	struct vec p0, float y0, struct vec v0, float dy0, struct vec a0,
	struct vec p1, float y1, struct vec v1, float dy1, struct vec a1)
{
	struct poly4d *p = &pp->pieces[0];
	p->duration = duration;
	pp->timescale = 1.0;
	pp->shift = vzero();
	pp->n_pieces = 1;

	{
        float tmp[POLY5_COEFFS];
        memcpy(tmp, p->p[0], sizeof(tmp));
        poly5(tmp, duration, p0.x, v0.x, a0.x, p1.x, v1.x, a1.x);
        memcpy(p->p[0], tmp, sizeof(tmp));
    }
    {
        float tmp[POLY5_COEFFS];
        memcpy(tmp, p->p[1], sizeof(tmp));
        poly5(tmp, duration, p0.y, v0.y, a0.y, p1.y, v1.y, a1.y);
        memcpy(p->p[1], tmp, sizeof(tmp));
    }
    {
        float tmp[POLY5_COEFFS];
        memcpy(tmp, p->p[2], sizeof(tmp));
        poly5(tmp, duration, p0.z, v0.z, a0.z, p1.z, v1.z, a1.z);
        memcpy(p->p[2], tmp, sizeof(tmp));
    }
    {
        float tmp[POLY5_COEFFS];
        memcpy(tmp, p->p[3], sizeof(tmp));
        poly5(tmp, duration, y0, dy0, 0, y1, dy1, 0);
        memcpy(p->p[3], tmp, sizeof(tmp));
    }
}

// y, dy == yaw, derivative of yaw
void piecewise_plan_7th_order_no_jerk(struct piecewise_traj *pp, float duration,
	struct vec p0, float y0, struct vec v0, float dy0, struct vec a0,
	struct vec p1, float y1, struct vec v1, float dy1, struct vec a1)
{
	struct poly4d *p = &pp->pieces[0];
	p->duration = duration;
	pp->timescale = 1.0;
	pp->shift = vzero();
	pp->n_pieces = 1;
	{
        float tmp[POLY7_COEFFS];
        memcpy(tmp, p->p[0], sizeof(tmp));
        poly7_nojerk(tmp, duration, p0.x, v0.x, a0.x, p1.x, v1.x, a1.x);
        memcpy(p->p[0], tmp, sizeof(tmp));
    }
    {
        float tmp[POLY7_COEFFS];
        memcpy(tmp, p->p[1], sizeof(tmp));
        poly7_nojerk(tmp, duration, p0.y, v0.y, a0.y, p1.y, v1.y, a1.y);
        memcpy(p->p[1], tmp, sizeof(tmp));
    }
    {
        float tmp[POLY7_COEFFS];
        memcpy(tmp, p->p[2], sizeof(tmp));
        poly7_nojerk(tmp, duration, p0.z, v0.z, a0.z, p1.z, v1.z, a1.z);
        memcpy(p->p[2], tmp, sizeof(tmp));
    }
    {
        float tmp[POLY7_COEFFS];
        memcpy(tmp, p->p[3], sizeof(tmp));
        poly7_nojerk(tmp, duration, y0, dy0, 0, y1, dy1, 0);
        memcpy(p->p[3], tmp, sizeof(tmp));
    }
}

