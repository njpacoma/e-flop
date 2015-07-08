#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <e-hal.h>
#include "common.h"

int main(int arcg, char *argv[])
{
	e_platform_t platform;
	e_epiphany_t dev;

	long n = INTERVALS;
	unsigned clr;
	clr = (unsigned)0x00000000;
	int done[CORES], all_done;
	double results[CORES];
	double pi;
	int i, j;

	double PI25DT = 3.141592653589793238462643;

	printf("number of intervals is %ld\n", n);

	printf("initialize the epiphany platform\n");
	e_init(NULL);

	printf("reset the epiphany platform\n");
	e_reset_system();

	printf("get the epiphany platform information\n");
	e_get_platform_info(&platform);

	printf("number of cores %d\n", platform.rows * platform.cols);

	printf("open an eCore workgroup--all cores\n");
	e_open(&dev, 0, 0, platform.rows, platform.cols);

	printf("initialize the done flags\n");
	for (i=0; i<platform.rows; i++) {
		for (j=0; j<platform.cols; j++) {
			e_write(&dev, i, j, 0x7000, &clr, sizeof(clr));
		}
	}

	printf("load the code into eCore workgroup\n");
	e_load_group("e_flop.srec", &dev, 0, 0, platform.rows, platform.cols, E_TRUE);

	printf("wait until all cores have completed their work\n");
	while(1) {
		all_done = 0;
		for (i=0; i<platform.rows; i++) {
			for (j=0; j<platform.cols; j++) {
				e_read(&dev, i, j, 0x7000, &done[i*platform.cols+j], sizeof(int));
				all_done += done[i*platform.cols+j];
			}
		}
		if (all_done == 16) {
			printf("all cores have finished\n");
			break;
		}
	}

	for (i=0; i<platform.rows; i++) {
		for (j=0; j<platform.cols; j++) {
			e_read(&dev, i, j, 0x6000, &results[i*platform.cols+j], sizeof(double));
			// printf("results[%d] %.16f\n", i*platform.cols+j, results[i*platform.cols+j]);
		}
	}

	pi = 0.0;
	for (i=0; i<CORES; i++) {
		pi += results[i];
	}

	printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
	// printf("pi is approximately %.16f\n", pi);

	printf("close the eCore workgroup\n");
	e_close(&dev);

	printf("release the epiphany platform\n");
	e_finalize();

	return EXIT_SUCCESS;
}
