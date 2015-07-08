#include <stdio.h>
#include "e-lib.h"
#include "common.h"

int main(void)
{
	e_coreid_t coreid;

	int n = INTERVALS;
	int i;
	unsigned my_row, my_col;
	double mypi, h, sum, x;
	double *partial = (void *)0x6000;
	unsigned *done = (void *)0x7000;

	// partial = (unsigned *) 0x6000;
	// done = (unsigned *) 0x7000;

	// (*(partial)) = 0.0;
	(*partial) = 0.0;

	coreid = e_get_coreid();
	e_coords_from_coreid(coreid, &my_row, &my_col);

	h = 1.0 / (double) n;
	sum = 0.0;

	for (i = (my_row * 4) + (my_col + 1); i <= n; i += CORES)
	{
		x = h * ((double)i - 0.5);
		sum += ( 4.0 / (1.0 + x * x ));
	}
	mypi = h * sum;

	// (*(partial)) = mypi;
	(*partial) = mypi;

	// (*(done)) = 0x00000001;
	(*done) = 0x00000001;

	__asm__ __volatile__("idle");
}
