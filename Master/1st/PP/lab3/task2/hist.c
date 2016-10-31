/*
* hist.c: Image histogram (histogram equalization).
*/
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>
#include <omp.h>

const uint64_t width = 32 * 1024;
const uint64_t height = 32 * 1024;

void *xmalloc(size_t size)
{
	void *p = malloc(size);
	if (p == NULL) {
		fprintf(stderr, "No enough memory\n");
		exit(EXIT_FAILURE);
	}
	return p;
}

void hist_serial(uint8_t *pixels, uint64_t height, uint64_t width)
{
	uint64_t npixels = height * width;

	// Number of each pixel occurrence in the image
	uint64_t *h = xmalloc(sizeof(*h) * 256);
	for (uint64_t i = 0; i < 256; i++)
		h[i] = 0;

	for (uint64_t i = 0; i < npixels; i++)
		h[pixels[i]]++;

	uint64_t mini, maxi;
	for (mini = 0; mini < 256 && h[mini] == 0; mini++);
	for (maxi = 255; maxi >= 0 && h[maxi] == 0; maxi--);

	uint64_t q = 255 / (maxi - mini);
	for (uint64_t i = 0; i < npixels; i++)
		pixels[i] = (pixels[i] - mini) * q;

	free(h);
}

void hist_omp(uint8_t *pixels, uint64_t height, uint64_t width)
{
	uint64_t npixels = height * width;

	// Number of each pixel occurrence in the image
	uint64_t *h = xmalloc(sizeof(*h) * 256);

	#pragma omp parallel
	{
		#pragma omp for
		for (uint64_t i = 0; i < 256; i++)
			h[i] = 0;

		uint64_t *h_loc = xmalloc(sizeof(*h_loc) * 256);

		#pragma omp for
		for (uint64_t i = 0; i < npixels; i++)
			h_loc[pixels[i]]++;

		#pragma omp critical
		for (uint64_t i = 0; i < 256; i++)
			h[i] += h_loc[i];

		free(h_loc);
	}

	uint64_t mini, maxi;
	for (mini = 0; mini < 256 && h[mini] == 0; mini++);
	for (maxi = 255; maxi >= 0 && h[maxi] == 0; maxi--);

	uint64_t q = 255 / (maxi - mini);

	#pragma omp parallel for
	for (uint64_t i = 0; i < npixels; i++)
		pixels[i] = (pixels[i] - mini) * q;

	free(h);
}

int main(int argc, char *argv[])
{
	printf("Histogram (image %" PRIu64 "x%" PRIu64 " ~ %" PRIu64 " MiB)\n",
		height, width, height * width / (1 << 20));
	uint64_t npixels = width * height;
	uint8_t *pixels1, *pixels2;

	// Run serial version
	pixels1 = xmalloc(sizeof(*pixels1) * npixels);
	srand(0);
	for (uint64_t i = 0; i < npixels; i++)
		pixels1[i] = rand() % 256;

	double tser = omp_get_wtime();
	hist_serial(pixels1, height, width);
	tser = omp_get_wtime() - tser;
	printf("Elapsed time (serial): %.6f\n", tser);

	// Run parallel version
	pixels2 = xmalloc(sizeof(*pixels2) * npixels);
	srand(0);
	for (uint64_t i = 0; i < npixels; i++)
		pixels2[i] = rand() % 256;

	double tpar = omp_get_wtime();
	hist_omp(pixels2, height, width);
	tpar = omp_get_wtime() - tpar;
	printf("Elapsed time (parallel): %.6f\n", tpar);

	printf("Speedup: %.2f\n", tser / tpar);

	for (uint64_t i = 0; i < npixels; i++) {
		if (pixels1[i] != pixels2[i]) {
			printf("Verification failed: %" PRIu64 " %d %d \n",
				i, pixels1[i], pixels2[i]);
			break;
		}
	}

	free(pixels1);
	free(pixels2);

	return 0;
}
