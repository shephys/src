/* Local similarity measure between two datasets. */
/*
  Copyright (C) 2004 University of Texas at Austin
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <rsf.h>	

#include "divn.h"

int main(int argc, char* argv[])
{
    bool verb;
    int dim, dim1, i, n1, i1, i2, n2, niter, n[SF_MAX_DIM]; 
    int rect[SF_MAX_DIM];
    char key[6];	
    double norm;
    float *one, *two, *rat1, *rat2;
    sf_file in, other, out;

    sf_init(argc,argv);
    in = sf_input("in");
    out = sf_output("out");
    other = sf_input("other");

    if (SF_FLOAT != sf_gettype(in) ||
        SF_FLOAT != sf_gettype(other)) sf_error("Need float input");

    dim = sf_filedims(in,n);	

    dim1 = -1;
    for (i=0; i < dim; i++) {
	snprintf(key,6,"rect%d",i+1);
	if (!sf_getint(key,rect+i)) rect[i]=1;
	/*( rect#=(1,1,...) smoothing radius on #-th axis )*/ 
	if (rect[i] > 1) dim1 = i+1;
    }

    n1 = n2 = 1;
    for (i=0; i < dim; i++) {
        if (i < dim1) {
            n1 *= n[i];
        } else {
            n2 *= n[i];
        }
    }

    if (!sf_getbool("verb",&verb)) verb=true;
    /* verbosity */

    if (!sf_getint("niter",&niter)) niter=20;
    /* maximum number of iterations */

    divn_init(dim1, n1, n, rect, niter, verb);
	
    one = sf_floatalloc(n1);
    two = sf_floatalloc(n1);
    rat1 = sf_floatalloc(n1);
    rat2 = sf_floatalloc(n1);

    for (i2=0; i2 < n2; i2++) {
	sf_warning(verb? 
		   "record %d of %d":
		   "record %d of %d;",i2+1,n2);

	sf_floatread(one,n1,in);
        sf_floatread(two,n1,other);
	
        /* first division */
        norm = sqrt(n1/cblas_dsdot( n1, two, 1, two, 1));	

        for (i1=0; i1 < n1; i1++) {
	    one[i1] *= norm;
	    two[i1] *= norm;
	}
	
	divn(one,two,rat1);

        /* second division */
	norm = sqrt(n1/cblas_dsdot( n1, one, 1, one, 1));	

        for (i1=0; i1 < n1; i1++) {
	    one[i1] *= norm;
	    two[i1] *= norm;
	}

        divn(two,one,rat2);

	/* combination */
	divn_combine (rat1,rat2,rat1);
        sf_floatwrite(rat1,n1,out);
    }
    if (!verb) sf_warning(".");

    exit(0);
}
