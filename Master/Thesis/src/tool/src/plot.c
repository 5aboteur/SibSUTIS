#include "plot.h"

extern int recv_cnt;

extern double *pmq_sz_plot_values;
extern double *umq_sz_plot_values;

extern double *pmq_ma_plot_values;
extern double *umq_ma_plot_values;

extern double *pmq_st_plot_values;
extern double *umq_st_plot_values;

void plot(void)
{
	/* Plotting queues lengths */

    gnuplot_ctrl *hsz = gnuplot_init();

	gnuplot_setstyle(hsz, "lines");
	gnuplot_set_xlabel(hsz, "NITERS");
	gnuplot_set_ylabel(hsz, "ENTRIES");

	gnuplot_plot_x(hsz, pmq_sz_plot_values, recv_cnt, "posted recvq length");
	gnuplot_plot_x(hsz, umq_sz_plot_values, recv_cnt, "unexpected recvq length");

	printf("Press <ENTER> to continue...");
//	while (getchar()!='\n') {}
	sleep(10);
    gnuplot_close(hsz);

	/* Plotting match attempts */

    gnuplot_ctrl *hma = gnuplot_init();

	gnuplot_setstyle(hma, "lines");
	gnuplot_set_xlabel(hma, "NITERS");
	gnuplot_set_ylabel(hma, "ATTEMPTS");

	gnuplot_plot_x(hma, pmq_ma_plot_values, recv_cnt, "posted recvq match attemts");
	gnuplot_plot_x(hma, umq_ma_plot_values, recv_cnt, "unexpected recvq match attempts");

	printf("Press <ENTER> to continue...");
//	while (getchar()!='\n') {}
	sleep(10);

    gnuplot_close(hma);

	/* Plotting spent time on search */

    gnuplot_ctrl *hst = gnuplot_init();

	gnuplot_setstyle(hst, "lines");
	gnuplot_set_xlabel(hst, "NITERS");
	gnuplot_set_ylabel(hst, "TIME");

	gnuplot_plot_x(hst, pmq_st_plot_values, recv_cnt, "posted recvq search time");
	gnuplot_plot_x(hst, umq_st_plot_values, recv_cnt, "unexpected recvq search time");

	printf("Press <ENTER> to continue...");
//	while (getchar()!='\n') {}
	sleep(10);

    gnuplot_close(hst);
}

