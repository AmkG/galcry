/*
inc/backend/voronoi.h - Voronoi/Delaunay graph creation.
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
#ifndef BACKEND_VORONOI_H_INCLUDED
#define BACKEND_VORONOI_H_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

/* Represents a Voronoi graph.  */
struct Voronoi_s;
typedef struct Voronoi_s Voronoi;

/* Construct a Voronoi graph.  The SITES array should be X and Y coordinates,
   alternating, and its size should be twice the value of NUM.  NUM should be
   3 or higher.  */
Voronoi *voronoi_new (double *sites, unsigned int num);
/* Delete a Voronoi graph, removing all its data.  */
void voronoi_delete (Voronoi *vp);

/* The Voronoi graph is accessed via a DCEL structure.  DCEL is represented by
   two object types: faces and halfedges.  A planar graph is composed of faces
   bounded by edges.  In the DCEL an edge is "split" into two halfedges.  A
   halfedge is attached to a particular face; a halfedge can access its face,
   and can access its "twin" halfedge of the other face.  Thus, to traverse
   the Voronoi graph we need only to provide a single face.  */

/* Halfedges in Voronoi can be either "bounded" (i.e. a line segment) or
   "unbounded" (a ray usually, a line in the degenerate case of 2 sites, which
   we don't support).  Halfedges are drawn in a counterclockwise manner, and
   can access their start and end points, if that point exists.  They can
   access the "next" and "previous" halfedge if they are bounded in that
   direction (the start point is the previous halfedge's end point). For
   example, for the top right face in the following Voronoi graph:

   >
   >   .  |  .
   >      |
   >   ---+
   >       \
   >   .    \
   >         \

   The downward halfedge has no previous halfedge, it has a next halfedge
   that is the diagonal halfedge.  */

/* Pointers to face and halfedge structures can be compared for equality.
   There is an arbitrary ordering on the pointers.  Deleting the Voronoi
   graph invalidates all face and halfedge data.  */
struct VoronoiFace_s;
typedef struct VoronoiFace_s VoronoiFace;
struct VoronoiHalfedge_s;
typedef struct VoronoiHalfedge_s VoronoiHalfedge;

/* Get an arbitrary face.  */
VoronoiFace *voronoi_get_face (Voronoi *vp);
/* True if the face is completely bounded, false otherwise.  */
bool voronoi_face_is_bounded (VoronoiFace *fp);
/* Get the site location in the face.  */
double voronoi_face_site_x (VoronoiFace *fp);
double voronoi_face_site_y (VoronoiFace *fp);

/* Get an arbitrary halfedge.  */
VoronoiHalfedge *voronoi_face_get_halfedge (VoronoiFace *fp);

/* Get a halfedge's twin, i.e. the halfedge on the other
   face.  */
VoronoiHalfedge *voronoi_halfedge_twin (VoronoiHalfedge *hp);
/* Get a halfedge's face.  */
VoronoiFace *voronoi_halfedge_face (VoronoiHalfedge *hp);
/* Get the previous halfedge, returns NULL if unbounded from
   its start.  */
VoronoiHalfedge *voronoi_halfedge_prev (VoronoiHalfedge *hp);
/* Get the next halfedge, or NULL if unbounded from its end.  */
VoronoiHalfedge *voronoi_halfedge_next (VoronoiHalfedge *hp);
/* Get the start point, or an arbitrary point on the ray if no
   start point.  */
double voronoi_halfedge_start_x (VoronoiHalfedge *hp);
double voronoi_halfedge_start_y (VoronoiHalfedge *hp);
/* Get the end point, or an arbitrary point on the ray if no
   end point.  */
double voronoi_halfedge_end_x (VoronoiHalfedge *hp);
double voronoi_halfedge_end_y (VoronoiHalfedge *hp);

/* Number of Voronoi faces and halfedges in total.  */
unsigned int voronoi_num_faces (Voronoi *vp);
unsigned int voronoi_num_halfedges (Voronoi *vp);

/* Access faces and halfedges by index.  */
VoronoiFace *voronoi_index_face (Voronoi *vp, unsigned int i);
VoronoiHalfedge *voronoi_index_halfedge (Voronoi *vp, unsigned int i);

/* Determine the index numbers of the faces and halfedges items.  */
unsigned int voronoi_face_index (VoronoiFace *fp);
unsigned int voronoi_halfedge_index (VoronoiHalfedge *fp);

#ifdef __cplusplus
};
#endif

#endif /* !defined (BACKEND_VORONOI_H_INCLUDED) */
