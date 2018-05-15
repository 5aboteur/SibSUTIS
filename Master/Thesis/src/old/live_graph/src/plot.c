#include "gnuplot_i.h"
#include "plot.h"

extern double plot_values[10000];
extern int recv_cnt;
extern int delay_ms;

void *plot(void *arg)
{
//	int i = 0;
	int total_msg_cnt = *((int *)arg);

	gnuplot_ctrl *h;
	h = gnuplot_init();

	gnuplot_setstyle(h, "lines");

	// wait at least 1 message will received
//	usleep(2);
//	while (!recv_cnt) { /*printf(".. %d\n", recv_cnt);*/ }

	while (recv_cnt < total_msg_cnt)
	{
//		printf("%d: %d\n", i++, recv_cnt);
		gnuplot_plot_x(h, plot_values, recv_cnt, "umq size");
		usleep(delay_ms * 10);
		gnuplot_resetplot(h);
	}

	printf("Closing plot thread.\n");

	sleep(5);

	free(arg);
	gnuplot_close(h);

	return NULL;
}

