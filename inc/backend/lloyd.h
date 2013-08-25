/*
inc/backend/lloyd.h - Lloyd Relaxation
Copyright (C) 2013 Alan Manuel K. Gloria

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef BACKEND_LLOYD_H_INCLUDED
#define BACKEND_LLOYD_H_INCLUDED

/* Performs Lloyd relaxation on the input set of points.

   Lloyd relaxation is a method of making an input set of points
   more regular.  The exact method involves creating a Voronoi
   graph from the points, and for each bounded site, getting the
   average of the edge points. */
void lloyd_relaxation (double *sites, unsigned int *pnum);

#endif /* !defined (BACKEND_LLOYD_H_INCLUDED) */
