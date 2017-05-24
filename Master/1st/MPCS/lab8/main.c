#include "ray.h"
#include "sphere.h"
#include "ellipsoid.h"

T_FL3 get_radiance_s(T_RAY *r, T_SPH *s);
T_FL3 get_radiance_e(T_RAY *r, T_ELP *s);

/* >>> STARTS HERE <<< */
int main(int argc, char *argv[])
{
	int i, j;

	FILE *fp;

	if (!(fp = fopen("res/grid.ppm", "w"))) {
		fprintf(stderr, "Cannot open the file.\n");
		exit(EXIT_FAILURE);
	}

	/* ---------- Grid parameters ---------- */

	int WIDTH  = 512;
	int HEIGHT = 512;

	T_FL3 lowleft, horiz, vert;

	/* ---------- Radiation source ---------- */

	T_FL3 origin, radiance;

	// Set the radiance origin
	init_point(&origin, 0.0f, 0.0f, 5.0f);

	/* ---------- Sphere ---------- */
/*
	T_SPH sphere;
	T_FL3 center;
	float radius;

	// Default center
	init_point(&center, 0.3f, 0.2f, 0.7f);

	// Default radius
	radius = 1.5f;

	// Init this on 3D scene
	init_point(&lowleft, -1.0f, -1.0f, -1.5f);
	init_point(&horiz, 2.0f, 0.0f, 0.0f);
	init_point(&vert, 0.0f, 2.0f, 0.0f);
	init_sphere(&sphere, &center, radius);
*/
	/* ---------- Ellipsoid ---------- */

	T_ELP ellipsoid;
	T_FL3 center;
	float a, b, c;

	// Default center
	init_point(&center, 0.3f, 0.2f, 0.7f);

	// Default radii
	a = 2.5f, b = 2.6f, c = 2.4f;

	// Init this on 3D scene
	init_point(&lowleft, -1.0f, -2.0f, -2.0f);
	init_point(&horiz, 2.0f, 0.0f, 0.0f);
	init_point(&vert, 0.0f, 4.0f, 0.0f);
	init_ellipsoid(&ellipsoid, &center, a, b, c);

	/* ---------- Ray tracing routine ---------- */

	T_FL3 woff, hoff, direction;
	T_RAY ray;

	// PPM parameters
	fprintf(fp, "P3\n%d %d\n255\n", WIDTH, HEIGHT);

	for (i = 0; i < HEIGHT; ++i) {
		for (j = 0; j < WIDTH; ++j) {
			// Offset values
			float w = (float)j / (float)WIDTH;
			float h = (float)i / (float)HEIGHT;

			// Direction of the ray comes from start
			// point + width offset + height offset
			woff = _kmul(&horiz, w);
			hoff = _kmul(&vert, h);

			direction = _add(&woff, &hoff);
			direction = _add(&lowleft, &direction);

			// Generate the ray
			init_ray(&ray, &origin, &direction);

			// Send it out
//			radiance = get_radiance_s(&ray, &sphere);
			radiance = get_radiance_e(&ray, &ellipsoid);

			// Convert the intensity to rgb colors
			int ir = (int)255.0f * radiance.x;
			int ig = (int)255.0f * radiance.y;
			int ib = (int)255.0f * radiance.z;

			fprintf(fp, "%d %d %d\n", ir, ig, ib);
		}
	}

	fclose(fp);
	return 0;
}

/* Calculate the intensisty of the radiance */
T_FL3 get_radiance_s(T_RAY *r, T_SPH *s)
{
	T_FL3 res;
	T_FL3 p;

	// Check if we got the intersection
	if (find_intersection_s(r, s, &p) == TRUE) {
		// Calculate the intensity by i=i0*e^(-kl)
		float intensity = 1.0f * exp(-1.0f *
			(-length(&r->direction) + p.z));

		// Don't like that super white..
		if (intensity >= 1.0f) intensity = 0.999f;

		res.z = res.y = res.x = intensity;
	} else {
		res.x = 0.999f;
		res.y = 0.999f;
		res.z = 0.999f;
	}

	return res;
}

/* Calculate the intensisty of the radiance */
T_FL3 get_radiance_e(T_RAY *r, T_ELP *e)
{
	T_FL3 res;
	T_FL3 p;

	// Check if we got the intersection
	if (find_intersection_e(r, e, &p) == TRUE) {

		// Calculate the intensity by i=i0*e^(-kl)
		float intensity = 1.0f * exp(-1.0f *
			(-length(&r->direction) + p.z));

		// Don't like that super white..
		if (intensity >= 1.0f) intensity = 0.999f;

		res.z = res.y = res.x = intensity;
	} else {
		res.x = 0.999f;
		res.y = 0.999f;
		res.z = 0.999f;
	}

	return res;
}
