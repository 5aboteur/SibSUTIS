#include "plot.h"

extern double *plot_values;
extern int recv_cnt;

void plot(void)
{
    gnuplot_ctrl *h = gnuplot_init();

    gnuplot_plot_once("UMQ Size",
					  "lines",
					  "niter",
					  "nmsg",
					  plot_values,
					  NULL,
					  recv_cnt);

    gnuplot_close(h);
}

