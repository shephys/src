#include <rsf.h>

#include "eno.h"

int main(int argc, char* argv[])
{
    int n1, n2, i1, nn1, i2, order, i;
    float o1, d1, oo1, dd1, *tin, *tout, f, f1;
    eno map;
    sf_file in, out;

    sf_init (argc, argv);
    in = sf_input ("in");
    out = sf_output("out");

    if (!sf_histint(in,"n1",&n1)) sf_error("No n1= in input");    
    if (!sf_histfloat(in,"d1",&d1)) sf_error("No d1= in input");
    if (!sf_histfloat(in,"o1",&o1)) sf_error("No o1= in input");
    n2 = sf_leftsize(in,1);

    if (!sf_getint("n1",&nn1)) nn1=n1;
    if (!sf_getfloat("d1",&dd1)) dd1=d1;
    if (!sf_getfloat("o1",&oo1)) oo1=o1;
    sf_putint(out,"n1",nn1);
    sf_putfloat(out,"d1",dd1);
    sf_putfloat(out,"o1",oo1);

    tin = sf_floatalloc(n1);
    tout = sf_floatalloc(nn1);

    if (!sf_getint("order",&order)) order=3;
    if (order > n1) order=n1;

    map = eno_init (order,n1);

    for (i2=0; i2 < n2; i2++) {
	sf_read(tin,sizeof(float),n1,in);
	eno_set (map,tin);

	for (i1=0; i1 < nn1; i1++) {
	    f = (oo1+i1*dd1-o1)/d1; i=f; f -= i;
	    eno_apply(map, i, f, tout+i1, &f1, FUNC);
	}
	sf_write(tout,sizeof(float),nn1,out);
    }

    exit (0);
}
