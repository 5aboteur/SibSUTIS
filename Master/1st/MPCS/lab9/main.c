#include "ray.h"
#include "sphere.h"
#include "ellipsoid.h"

T_FL3 get_radiance_s(T_RAY *r, T_SPH *s);
T_FL3 get_radiance_e(T_RAY *r, T_ELP *s);

/* >>> STARTS HERE <<< */
int main(int argc, char *argv[])
{
	int i, j;

	FILE *fp1, *fp2, *fp3;

	if (!(fp1 = fopen("res/grid.ppm", "w"))) {
		fprintf(stderr, "Cannot open the file.\n");
		exit(EXIT_FAILURE);
	}

	if (!(fp2 = fopen("res/grid_noisy.ppm", "w+"))) {
		fprintf(stderr, "Cannot open the file.\n");
		exit(EXIT_FAILURE);
	}

	if (!(fp3 = fopen("res/grid_filtered.ppm", "w"))) {
		fprintf(stderr, "Cannot open the file.\n");
		exit(EXIT_FAILURE);
	}

	srand48((unsigned)time(NULL));

	/* ---------- Grid parameters ---------- */

	int WIDTH  = 512;
	int HEIGHT = 512;

	T_FL3 lowleft, horiz, vert;

	init_point(&lowleft, -1.0f, -2.0f, -2.0f);
	init_point(&horiz, 2.0f, 0.0f, 0.0f);
	init_point(&vert, 0.0f, 4.0f, 0.0f);

	/* ---------- Radiation source ---------- */

	T_FL3 origin, radiance, radiance_e1, radiance_e2;

	// Set the radiance origin
	init_point(&origin, 0.0f, 0.0f, 5.0f);

	/* ---------- Ellipsoid 1 ---------- */
	T_ELP ellipsoid_1;
	T_FL3 center_e1;
	float a1, b1, c1;

	// Default center
	init_point(&center_e1, 0.3f, 0.2f, 0.7f);

	// Default radii
	a1 = 2.6f, b1 = 2.7f, c1 = 2.5f;

	// Init this on 3D scene
	init_ellipsoid(&ellipsoid_1, &center_e1, a1, b1, c1);

	/* ---------- Ellipsoid 2 ---------- */

	T_ELP ellipsoid_2;
	T_FL3 center_e2;
	float a2, b2, c2;

	// Default center
	init_point(&center_e2, 0.3f, 0.2f, 0.7f);

	// Default radii
	a2 = 2.5f, b2 = 2.6f, c2 = 2.4f;

	// Init this on 3D scene
	init_ellipsoid(&ellipsoid_2, &center_e2, a2, b2, c2);

	/* ---------- Ray tracing routine ---------- */

	T_FL3 woff, hoff, direction;
	T_RAY ray;

	// PPM parameters
	fprintf(fp1, "P3\n%d %d\n255\n", WIDTH, HEIGHT);
	fprintf(fp2, "P3\n%d %d\n255\n", WIDTH, HEIGHT);
	fprintf(fp3, "P3\n%d %d\n255\n", WIDTH, HEIGHT);

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
			radiance_e1 = get_radiance_e(&ray, &ellipsoid_1);
			radiance_e2 = get_radiance_e(&ray, &ellipsoid_2);

//			printf("%f,%f\n", radiance_e1.x, radiance_e2.x);

			radiance = _mul(&radiance_e1, &radiance_e2);

			// Convert the intensity to rgb colors
			int ir = (int)255.0f * radiance.x;
			int ig = (int)255.0f * radiance.y;
			int ib = (int)255.0f * radiance.z;

			// Noise addition
			int ir_n = (int)((ir - 55) + rand() % ir);// % 255;
			int ig_n = (int)((ig - 55) + rand() % ig);// % 255;
//			int ig_n = 13;
			int ib_n = (int)((ib - 55) + rand() % ib);// % 255;

			fprintf(fp1, "%d %d %d\n", ir, ig, ib);
			fprintf(fp2, "%d %d %d\n", ir_n, ig_n, ib_n);
		}
	}

	fclose(fp1);

	// Set pointer to default position
	rewind(fp2);

	// Goto rgb sequence
	fseek(fp2, 16, SEEK_CUR);

	/* ---------- FILTERING ---------- */

	// Average value of 3x3 block
	int avg = 0;

	// Read '3 x WIDTH' data block, each pixel contains
	// 3 values (rgb), so that's why there 9 (3*3) :)
	int TRINE = 9 * WIDTH;

	// Filtering routine
	for (i = 0; i < HEIGHT; i += 3) {

		// For rgb values
		int buf[TRINE + 1];

		// Read them
		for (j = 0; j < TRINE; ++j)
			fscanf(fp2, "%d", &buf[j]);

		// Average them
		for (j = 0; j < TRINE / 3; j += 9) {
			avg += buf[j];
			avg += buf[j + 1];
			avg += buf[j + 2];
			avg += buf[j + 3];
			avg += buf[j + 4];
			avg += buf[j + 5];
			avg += buf[j + 6];
			avg += buf[j + 7];
			avg += buf[j + 8];

			avg += buf[j + WIDTH * 3];
			avg += buf[j + WIDTH * 3 + 1];
			avg += buf[j + WIDTH * 3 + 2];
			avg += buf[j + WIDTH * 3 + 3];
			avg += buf[j + WIDTH * 3 + 4];
			avg += buf[j + WIDTH * 3 + 5];
			avg += buf[j + WIDTH * 3 + 6];
			avg += buf[j + WIDTH * 3 + 7];
			avg += buf[j + WIDTH * 3 + 8];

			avg += buf[j + WIDTH * 6];
			avg += buf[j + WIDTH * 6 + 1];
			avg += buf[j + WIDTH * 6 + 2];
			avg += buf[j + WIDTH * 6 + 3];
			avg += buf[j + WIDTH * 6 + 4];
			avg += buf[j + WIDTH * 6 + 5];
			avg += buf[j + WIDTH * 6 + 6];
			avg += buf[j + WIDTH * 6 + 7];
			avg += buf[j + WIDTH * 6 + 8];

			avg /= 27;

			buf[j] = avg;
			buf[j + 1] = avg;
			buf[j + 2] = avg;
			buf[j + 3] = avg;
			buf[j + 4] = avg;
			buf[j + 5] = avg;
			buf[j + 6] = avg;
			buf[j + 7] = avg;
			buf[j + 8] = avg;

			buf[j + WIDTH * 3] = avg;
			buf[j + WIDTH * 3 + 1] = avg;
			buf[j + WIDTH * 3 + 2] = avg;
			buf[j + WIDTH * 3 + 3] = avg;
			buf[j + WIDTH * 3 + 4] = avg;
			buf[j + WIDTH * 3 + 5] = avg;
			buf[j + WIDTH * 3 + 6] = avg;
			buf[j + WIDTH * 3 + 7] = avg;
			buf[j + WIDTH * 3 + 8] = avg;

			buf[j + WIDTH * 6] = avg;
			buf[j + WIDTH * 6 + 1] = avg;
			buf[j + WIDTH * 6 + 2] = avg;
			buf[j + WIDTH * 6 + 3] = avg;
			buf[j + WIDTH * 6 + 4] = avg;
			buf[j + WIDTH * 6 + 5] = avg;
			buf[j + WIDTH * 6 + 6] = avg;
			buf[j + WIDTH * 6 + 7] = avg;
			buf[j + WIDTH * 6 + 8] = avg;

//			printf("AVG: %d, ITER: %d\n", avg, j);

			avg = 0;
		}

		// Dump them
		for (j = 0; j < TRINE; j += 3)
			fprintf(fp3, "%d %d %d\n", buf[j],
				buf[j + 1], buf[j + 2]);
	}

	fclose(fp2);
	fclose(fp3);

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
