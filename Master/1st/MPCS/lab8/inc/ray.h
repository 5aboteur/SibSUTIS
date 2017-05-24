#ifndef _RAY_H_
#define _RAY_H_

#include "float3.h"

/* A 'ray' as it is */
typedef struct ray {
	T_FL3 origin;		// where it starts
	T_FL3 direction;	// where it goes
} T_RAY;

/* Some getters as always */
T_FL3 get_origin(T_RAY *r) { return r->origin; }
T_FL3 get_direction(T_RAY *r) { return r->direction; }

/* Initialize the ray (set origin and direction) */
void init_ray(T_RAY *r, T_FL3 *o, T_FL3 *d)
{
	r->origin.x = o->x;
	r->origin.y = o->y;
	r->origin.z = o->z;

	r->direction.x = d->x;
	r->direction.y = d->y;
	r->direction.z = d->z;
}

/*
 * Think of a ray as a function 'F(k) = R.o + k*R.d',
 * where F - a 3D position along a line, R.o - origin
 * and R.d - direction of the ray.
 * The 'k' value is the parameter that allows us to
 * manipulate the ray in any directions.
 */
T_FL3 get_ray(T_RAY *r, float k)
{
	T_FL3 res;

	res.x = r->origin.x + k * r->direction.x;
	res.y = r->origin.y + k * r->direction.y;
	res.z = r->origin.z + k * r->direction.z;

	return res;
}

/* Display the ray (as it's origin and direction) */
void dump_ray(T_RAY *r)
{
	printf("O(%.2f, %.2f, %.2f) -> D(%.2f, %.2f, %.2f)\n",
		r->origin.x, r->origin.y, r->origin.z,
		r->direction.x, r->direction.y, r->direction.z);
}

#endif
