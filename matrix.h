#ifndef _MATRIX_H_INCLUDED_
#define _MATRIX_H_INCLUDED_
 
 /* Copyright (C) 2008 Pere Negre
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */



#include <stdio.h>
#include <stdlib.h>


typedef struct {
	float *d;
	int h,v;
} matrix_t;


extern matrix_t *   matrixNew(int h, int v);
extern void         matrixFree(matrix_t *m);
extern float        matrixGetElement(matrix_t *m, int i, int j);
extern void         matrixSetElement(matrix_t *m, float e, int i, int j);
extern void         matrixInverse(matrix_t *m);
extern matrix_t *   matrixMultiply(matrix_t *m, matrix_t *n);










#endif


 
