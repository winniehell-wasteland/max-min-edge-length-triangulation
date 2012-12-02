#ifndef MMT_INTERSECTION_ARR_INTERSECTION_ALGORITHM_H
#define MMT_INTERSECTION_ARR_INTERSECTION_ALGORITHM_H

#include "config.h"

#include <CGAL/Arr_extended_dcel.h>

#include "intersection_graph.h"
#include "cgal_utils.h"

class ArrIntersectionAlgorithm
{
  struct Vertex_data
  {
    Vertex_data() :
      is_input(false)
    {

    }

    bool is_input;
  };

  typedef CGAL::Arr_vertex_base<Point>                         Vertex_base;
  typedef CGAL::Arr_extended_vertex<Vertex_base, Vertex_data>  Extended_vertex;

  typedef CGAL::Arr_halfedge_base<Segment>                     Halfedge_base;

  typedef CGAL::Arr_dcel_base<Extended_vertex,
                              Halfedge_base,
                              CGAL::Arr_face_base>         Dcel;
  typedef CGAL::Arrangement_2<SegmentTraits, Dcel>         Arrangement_2;

public:
  ArrIntersectionAlgorithm(IntersectionGraph* graph, const PointSet& points) :
    arrangement_()
  {
    CGAL_precondition_msg(graph != 0, "IntersectionGraph has to be initialized!");

    // insert points
    for(PointSet::const_iterator point = points.begin(); point != points.end(); ++point)
      {
        Arrangement_2::Vertex_handle vertex = CGAL::insert_point(arrangement_, *point);
        vertex->data().is_input = true;
      }

    // insert segments
    CGAL::insert(arrangement_, graph->segments().begin(), graph->segments().end());

    for(Arrangement_2::Vertex_const_iterator vertex = arrangement_.vertices_begin();
        vertex != arrangement_.vertices_end();
        ++vertex)
      {
        if(vertex->data().is_input)
          {

          }
        else
          {
            Arrangement_2::Halfedge_around_vertex_const_circulator first = vertex->incident_halfedges(),
                current = first;

            do
              {
                logger->info(msg("curve: %1").arg(to_string(current->curve())));
                ++current;
              } while((current->curve().source() != first->curve().source())
                      && (current->curve().target() != first->curve().target()));
          }
      }
  }

private:
  Arrangement_2 arrangement_;
};

#endif // MMT_INTERSECTION_ARR_INTERSECTION_ALGORITHM_H
