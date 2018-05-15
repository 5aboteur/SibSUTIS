#include "plot.h"

extern double *umq_plot_values;
extern double *pmq_plot_values;

extern int umq_recv_cnt;
extern int pmq_recv_cnt;

void plot(void)
{
    gnuplot_ctrl *h = gnuplot_init();

	gnuplot_setstyle(h, "lines");
	gnuplot_set_xlabel(h, "NITERS");
	gnuplot_set_ylabel(h, "QSIZE");

	gnuplot_plot_x(h, umq_plot_values, umq_recv_cnt, "unexpected recvq");
	gnuplot_plot_x(h, pmq_plot_values, pmq_recv_cnt, "posted recvq");

	printf("Press <ENTER> to continue...");
	while (getchar()!='\n') {}

    gnuplot_close(h);
}

