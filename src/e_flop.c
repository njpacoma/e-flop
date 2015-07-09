#include <stdio.h>
#include "e-lib.h"
#include "common.h"

int main(void)
{
	e_coreid_t coreid;

	long n = INTERVALS;
	long i;
	unsigned my_row, my_col;
	float mypi, h, sum, x;
	float *partial = (void *)0x6000;
	unsigned *done = (void *)0x7000;

	(*partial) = (float)0.0;

	coreid = e_get_coreid();
	e_coords_from_coreid(coreid, &my_row, &my_col);

	h = (float)1.0 / (float) n;
	sum = (float)0.0;

	for (i = (my_row * 4) + (my_col + 1); i <= n; i += CORES)
	{
		x = h * ((float)i - (float)0.5);
		sum += ( (float)4.0 / ((float)1.0 + x * x ));
	}
	mypi = h * sum;

	(*partial) = mypi;

	(*done) = 0x00000001;

	__asm__ __volatile__("idle");
}
