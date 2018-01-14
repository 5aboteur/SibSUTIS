#include "gnuplot_i.h"
#include "plot.h"

//extern FILE *fp;
extern double plot_values[10000];
extern int CUR_ITER;

void *plot(void *arg)
{
	int i;

	gnuplot_ctrl *h;
	h = gnuplot_init();

	gnuplot_setstyle(h, "lines");

	sleep(2);

	while (CUR_ITER + 1 != 3000)
	{
		printf("%d\n", CUR_ITER);
		gnuplot_plot_x(h, plot_values, CUR_ITER, "unexpected messages queue size");
		usleep(10000);
		gnuplot_resetplot(h);
	}

	sleep(10);

	gnuplot_close(h);

	return NULL;
}

