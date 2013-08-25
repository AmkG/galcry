/*
src/backend/lloyd.c - Lloyd Relaxation
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
#include"all_config.h"

#include"backend/lloyd.h"
#include"backend/voronoi.h"

void
lloyd_relaxation (double *points, unsigned int *pnum)
{
  Voronoi *v;
  unsigned int i;

  v = voronoi_new (points, *pnum);
  *pnum = voronoi_num_faces (v);
  for (i = 0; i < *pnum; ++i)
    {
      VoronoiFace *f;
      double x;
      double y;

      f = voronoi_index_face (v, i);
      if (voronoi_face_is_bounded (f))
	{
	  VoronoiHalfedge *begin;
	  VoronoiHalfedge *h;
	  unsigned int points;

	  points = 0;
	  x = 0;
	  y = 0;
	  h = begin = voronoi_face_get_halfedge (f);
	  do
	    {
	      x += voronoi_halfedge_start_x (h);
	      y += voronoi_halfedge_start_y (h);
	      ++points;
	      h = voronoi_halfedge_next (h);
	    }
	  while (h != begin);
	  x = x / points;
	  y = y / points;
	}
      else
	{
	  x = voronoi_face_site_x (f);
	  y = voronoi_face_site_y (f);
	}

      points[i * 2] = x;
      points[i * 2 + 1] = y;
    }

  voronoi_delete (v);
}
