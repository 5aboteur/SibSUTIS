#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "float3.h"

/*
 * To represent a sphere on the scene, we
 * just need to know its center and radius
 */
typedef struct sphere {
	T_FL3 center;
	float radius;
} T_SPH;

T_FL3 get_center_s(T_SPH *s) { return s->center; }

/* Initialize the sphere */
void init_sphere(T_SPH *s, T_FL3 *c, float r)
{
	// Set the center
	s->center.x = c->x;
	s->center.y = c->y;
	s->center.z = c->z;

	// Set the radius
	s->radius = r;
}

/*
 * With that two parameters, we can easily find the
 * intersection point between the ray and the sphere.
 * In vector algebra, sphere equation changes
 *
 *  from  x^2 + y^2 + z^2 = r^2 (r - radius) to
 *
 * t^2*dot(R.d,R.d) + 2t*dot(R.o-C,R.o-C) + dot(C,C) = r^2
 *     ---- A -----   -  ------ B -------   --- C --   ---
 *
 * where R.d - ray direction, R.o - origin.
 * Parameter 't' is what we need to know to catch the
 * intersection. Solve the quadratic equation to get it.
 */
bool find_intersection_s(T_RAY *r, T_SPH *s, T_FL3 *p)
{
	T_FL3 ro = get_origin(r);	 // ray origin
	T_FL3 rd = get_direction(r); // ray direction
	T_FL3 sc = get_center_s(s);	 // center of the sphere
	T_FL3 oc = _sub(&ro, &sc);	 // R.o - C (center)

	float A = _dot(&rd, &rd);
	float B = 2.0f * _dot(&oc, &rd);
	float C = _dot(&oc, &oc) - s->radius * s->radius;

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
