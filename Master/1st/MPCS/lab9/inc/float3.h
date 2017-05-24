#ifndef _FLOAT3_H_
#define _FLOAT3_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

/* Boolean stuff */
typedef int bool;
#define TRUE  1
#define FALSE 0

/* Vector 3D coordinates */
typedef struct float3 {
	float x;	// x coord
	float y;	// y coord
	float z;	// z coord
} T_FL3;

/* Got getters if we want to grab them */
float getx(T_FL3 *f) { return f->x; }
float gety(T_FL3 *f) { return f->y; }
float getz(T_FL3 *f) { return f->z; }

/* Compute the length of the vector */
float length(T_FL3 *f)
{
	return sqrt(f->x * f->x +
				f->y * f->y +
				f->z * f->z);
}

/* Initialize the point coordinates */
void init_point(T_FL3 *f, float x, float y, float z)
{
	f->x = x; f->y = y; f->z = z;
}

/* Get the point thru summarize of two vectors */
T_FL3 _add(T_FL3 *f1, T_FL3 *f2)
{
	T_FL3 res;

	res.x = f1->x + f2->x;
	res.y = f1->y + f2->y;
	res.z = f1->z + f2->z;

	return res;
}

/* Get the point thru substruction of two vectors */
T_FL3 _sub(T_FL3 *f1, T_FL3 *f2)
{
	T_FL3 res;

	res.x = f1->x - f2->x;
	res.y = f1->y - f2->y;
	res.z = f1->z - f2->z;

	return res;
}

/* Get the point thru multiplication of two vectors */
T_FL3 _mul(T_FL3 *f1, T_FL3 *f2)
{
	T_FL3 res;

	res.x = f1->x * f2->x;
	res.y = f1->y * f2->y;
	res.z = f1->z * f2->z;

	return res;
}

/* Get the point thru division of two vectors */
T_FL3 _div(T_FL3 *f1, T_FL3 *f2)
{
	T_FL3 res;

	res.x = f1->x / f2->x;
	res.y = f1->y / f2->y;
	res.z = f1->z / f2->z;

	return res;
}

/* Multiply each coordinate by 'k' value */
T_FL3 _kmul(T_FL3 *f, float k)
{
	T_FL3 res;

	res.x = f->x * k;
	res.y = f->y * k;
	res.z = f->z * k;

	return res;
}

/* Vectors dot product */
float _dot(T_FL3 *f1, T_FL3 *f2)
{
	return (f1->x * f2->x +
			f1->y * f2->y +
			f1->z * f2->z);
}

/* Display the point coordinates */
void dump_point(T_FL3 *f)
{
	printf("(%.2f, %.2f, %.2f)\n",
		f->x, f->y, f->z);
}

#endif
