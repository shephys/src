/* Helical convolution. */
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

#include "chelicon.h"

#include "chelix.h"
/*^*/

static cfilter aa;

void chelicon_init( cfilter bb) 
/*<  Initialized with the filter. >*/
{
    aa = bb;
}

void chelicon_lop( bool adj, bool add, 
		   int nx, int ny, sf_complex* xx, sf_complex* yy) 
/*< linear operator >*/
{
    int ia, iy, ix;
    
    sf_ccopy_lop(adj, add, nx, nx, xx, yy);

    for (ia = 0; ia < aa->nh; ia++) {
	for (iy = aa->lag[ia]; iy < nx; iy++) {
	    if( aa->mis != NULL && aa->mis[iy]) continue;
	    ix = iy - aa->lag[ia];
	    if(adj) {
#ifdef SF_HAS_COMPLEX_H
		xx[ix] += yy[iy] * conjf(aa->flt[ia]);
#else
		xx[ix] = sf_cadd(xx[ix],sf_cmul(yy[iy],conjf(aa->flt[ia])));
#endif
	    } else {
#ifdef SF_HAS_COMPLEX_H
		yy[iy] += xx[ix] * aa->flt[ia];
#else
		yy[iy] = sf_cadd(yy[iy],sf_cmul(xx[ix],aa->flt[ia]));
#endif
	    }
	}
    }
}

/* 	$Id: helicon.c 2523 2007-02-02 16:45:29Z sfomel $	 */
