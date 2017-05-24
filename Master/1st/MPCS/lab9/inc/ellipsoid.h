#ifndef _ELLIPSOID_H_
#define _ELLIPSOID_H_

#include "float3.h"

/* Ellipsoid entity */
typedef struct ellipsoid {
	T_FL3 center;

	float a;
	float b;
	float c;
} T_ELP;

T_FL3 get_center_e(T_ELP *e) { return e->center; }

/* Initialize the ellipsoid */
void init_ellipsoid(T_ELP *e, T_FL3 *ec,
	float a, float b, float c)
{
	// Set the center
	e->center.x = ec->x;
	e->center.y = ec->y;
	e->center.z = ec->z;

	// Set the radii
	e->a = a;
	e->b = b;
	e->c = c;
}

/* Same case as the intersection with sphere but now this
 *
 * t^2*dot(R.d,R.d) + 2t*dot(R.o-C,R.o-C) + dot(C,C) = r^2
 *     ---- A -----   -  ------ B -------   --- C --   ---
 *
 * equation changes to this
 *
 * t^2*dot(R.d,R.d)/a^2 + 2t*dot(R.o-C,R.o-C)/b^2 + dot(C,C)/c^2 = 1
 *     ---- A ---------      ------ B -----------   ----- C ----   -
 *
 * where a, b, c - ellipsoid radii
 */
bool find_intersection_e(T_RAY *r, T_ELP *e, T_FL3 *p)
{
	T_FL3 ro = get_origin(r);	 // ray origin
	T_FL3 rd = get_direction(r); // ray direction
	T_FL3 sc = get_center_e(e);	 // center of the ellipsoid
	T_FL3 oc = _sub(&ro, &sc);	 // R.o - C

	float A = _dot(&rd, &rd) / (e->a * e->a);
	float B = 2.0f * _dot(&oc, &rd) / (e->b * e->b);
	float C = _dot(&oc, &oc) / (e->c * e->c) - 1.0f;

	float D = B*B - 4.0f*A*C;	 // discriminant

	// Got at least one intersection
	if (D > 0) {
		// Find the root
		float d1 = (-B - sqrt(D)) / (2.0f * A);
		float d2 = (-B + sqrt(D)) / (2.0f * A);

		// We need only nearest value
		float root = (d1 < d2) ? d1 : d2;

		// Get the ray which ends
		// in the intersection point
		*p = get_ray(r, root);

		return TRUE;
	}

	// No intersections
	return FALSE;
}

#endif
