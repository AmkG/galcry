/*
src/backend/voronoi.cxx - Voronoi/Delaunay graph creation.
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

/* Needed on my system, the installed library apparently does not
   even incude definitions for these.  */
#define CGAL_NO_ASSERTIONS
#define CGAL_NO_PRECONDITIONS
#define CGAL_NO_POSTCONDITIONS

#include"backend/voronoi.h"

#include<assert.h>
#include<boost/scoped_ptr.hpp>
#include<map>
#include<vector>

#include<CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include<CGAL/Delaunay_triangulation_2.h>
#include<CGAL/Voronoi_diagram_2.h>
#include<CGAL/Delaunay_triangulation_adaptation_policies_2.h>
#include<CGAL/Delaunay_triangulation_adaptation_traits_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Delaunay_triangulation_2<K>  Delaunay;
typedef CGAL::Delaunay_triangulation_adaptation_traits_2<Delaunay>
					   Voronoi_traits;
typedef CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<Delaunay>
					   Voronoi_policy;
typedef CGAL::Voronoi_diagram_2<Delaunay,Voronoi_traits,Voronoi_policy>
					   Voronoi_graph;

typedef Voronoi_graph::Face                Voronoi_face;
typedef Voronoi_graph::Halfedge            Voronoi_halfedge;
typedef Voronoi_graph::Vertex              Voronoi_vertex;

typedef K::Point_2                         Point_2;

struct VoronoiHalfedge_s
{
  VoronoiHalfedge *twin;
  VoronoiFace *face;
  VoronoiHalfedge *prev;
  VoronoiHalfedge *next;
  Point_2 start;
  Point_2 end;
  unsigned int idx;
};

struct VoronoiFace_s
{
  Point_2 site;
  VoronoiHalfedge *halfedge;
  bool is_bounded;
  unsigned int idx;
};

struct Voronoi_s
{
  std::vector<VoronoiHalfedge> halfedges;
  std::vector<VoronoiFace> faces;
};

/* Gets the equivalent structure from a CGAL value.  */
class CGALConverter
{
  /* Map from CGAL objects to our object types.  */
  std::map<Voronoi_halfedge, VoronoiHalfedge *> halfedgemap;
  std::map<Voronoi_face, VoronoiFace *> facemap;
  Voronoi *rv;
  /* Computes an arbitrary point, for ray-type halfedges.  */
  Point_2 arbitrary_point (Voronoi_halfedge const &inp)
  {
    double x1;
    double y1;
    double x2;
    double y2;

    x1 = inp.face ()->dual ()->point ().x ();
    y1 = inp.face ()->dual ()->point ().y ();
    x2 = inp.twin ()->face ()->dual ()->point ().x ();
    y2 = inp.twin ()->face ()->dual ()->point ().y ();

    return Point_2 ((x1 + x2) / 2, (y1 + y2) / 2);
  }
public:
  CGALConverter (Voronoi *rv_) : rv (rv_) { }
  VoronoiHalfedge *convert (Voronoi_halfedge const &inp)
  {
    unsigned int i;

    if (halfedgemap.count (inp))
      return halfedgemap[inp];

    rv->halfedges.push_back (VoronoiHalfedge ());
    i = rv->halfedges.size () - 1;
    halfedgemap[inp] = &rv->halfedges[i];

    rv->halfedges[i].twin = convert (*inp.twin ());
    rv->halfedges[i].face = convert (*inp.face ());
    rv->halfedges[i].prev =
      inp.has_source () ? convert (*inp.previous ()) :
      /*otherwise*/       0 ;
    rv->halfedges[i].next =
      inp.has_target () ? convert (*inp.next ()) :
      /*otherwise*/       0 ;
    rv->halfedges[i].start =
      inp.has_source () ? inp.source ()->point () :
      /*otherwise*/       arbitrary_point (inp);
    rv->halfedges[i].end =
      inp.has_target () ? inp.target ()->point () :
      /*otherwise*/       arbitrary_point (inp);
    rv->halfedges[i].idx = i;

    return &rv->halfedges[i];
  }
  VoronoiFace *convert (Voronoi_face const &inp)
  {
    unsigned int i;

    if (facemap.count (inp))
      return facemap[inp];

    rv->faces.push_back (VoronoiFace ());
    i = rv->faces.size () - 1;
    facemap[inp] = &rv->faces[i];

    rv->faces[i].site = inp.dual ()->point ();
    rv->faces[i].halfedge = convert (*inp.halfedge ());
    rv->faces[i].is_bounded = !inp.is_unbounded ();
    rv->faces[i].idx = i;

    return &rv->faces[i];
  }

};

Voronoi *
voronoi_new (double *sites, unsigned int num)
{
  /* The basic CGAL graph.  */
  boost::scoped_ptr<Voronoi_graph> pgraph;

  Voronoi *rv;

  unsigned int i;

  assert (num >= 3);

  /* Construct the CGAL graph.  */
  {
    std::vector<Point_2> points;
    Delaunay delaunay;

    for (i = 0; i < num; ++i)
      points.push_back (Point_2 (sites[i * 2], sites[i * 2 + 1]));
    delaunay.insert (points.begin (), points.end ());
    pgraph.reset (new Voronoi_graph (delaunay, true));
  }

  /* Construct our return value.  */
  rv = new Voronoi;
  /* Reserve space so that we don't reallocate the space.  */
  rv->halfedges.resize (pgraph->number_of_halfedges ());
  rv->faces.resize (pgraph->number_of_faces ());

  /* Converting the first is enough, as it will then do a kind of
     mark-and-sweep.  */
  {
    CGALConverter cvx (rv);
    (void) cvx.convert (*pgraph->faces_begin ());
  }

  return rv;
}

void
voronoi_delete (Voronoi *vp)
{
  delete vp;
}

VoronoiFace *
voronoi_get_face (Voronoi *vp)
{
  return &vp->faces[0];
}

bool
voronoi_face_is_bounded (VoronoiFace *fp)
{
  return fp->is_bounded;
}
double
voronoi_face_site_x (VoronoiFace *fp)
{
  return fp->site.x ();
}
double
voronoi_face_site_y (VoronoiFace *fp)
{
  return fp->site.y ();
}

VoronoiHalfedge *
voronoi_face_get_halfedge (VoronoiFace *fp)
{
  return fp->halfedge;
}

VoronoiHalfedge *
voronoi_halfedge_twin (VoronoiHalfedge *hp)
{
  return hp->twin;
}
VoronoiFace *
voronoi_halfedge_face (VoronoiHalfedge *hp)
{
  return hp->face;
}
VoronoiHalfedge *
voronoi_halfedge_prev (VoronoiHalfedge *hp)
{
  return hp->prev;
}
VoronoiHalfedge *
voronoi_halfedge_next (VoronoiHalfedge *hp)
{
  return hp->next;
}

double
voronoi_halfedge_start_x (VoronoiHalfedge *hp)
{
  return hp->start.x ();
}
double
voronoi_halfedge_start_y (VoronoiHalfedge *hp)
{
  return hp->start.y ();
}
double
voronoi_halfedge_end_x (VoronoiHalfedge *hp)
{
  return hp->end.x ();
}
double
voronoi_halfedge_end_y (VoronoiHalfedge *hp)
{
  return hp->end.y ();
}

unsigned int
voronoi_num_faces (Voronoi *vp)
{
  return vp->faces.size ();
}
unsigned int
voronoi_num_halfedges (Voronoi *vp)
{
  return vp->halfedges.size ();
}

VoronoiFace *
voronoi_index_face (Voronoi *vp, unsigned int i)
{
  return &vp->faces[i];
}
VoronoiHalfedge *
voronoi_index_halfedge (Voronoi *vp, unsigned int i)
{
  return &vp->halfedges[i];
}

unsigned int
voronoi_face_index (VoronoiFace *fp)
{
  return fp->idx;
}
unsigned int
voronoi_halfedge_index (VoronoiHalfedge *hp)
{
  return hp->idx;
}
