#ifndef MMT_INTERSECTION_NAIVE_INTERSECTION_ALGORITHM_H
#define MMT_INTERSECTION_NAIVE_INTERSECTION_ALGORITHM_H

#include <CGAL/intersections.h>

#include "utils.h"

#include "intersection_graph.h"

class NaiveIntersectionAlgorithm
{
public:
  NaiveIntersectionAlgorithm(IntersectionGraph* graph, const PointSet& points)
  {
    CGAL_precondition_msg(graph != 0, "IntersectionGraph has to be initialized!");

    for(IntersectionGraph::SegmentIterator s = graph->segments_begin();
        s != graph->segments_end(); ++s)
      {
        IntersectionGraph::SegmentIterator t = s;
        ++t;

        for( ; t != graph->segments_end(); ++t)
          {
            CGAL::Object result = CGAL::intersection(
              static_cast<CGAL::Segment_2<Kernel> >(*s),
              static_cast<CGAL::Segment_2<Kernel> >(*t)
            );

            if(const CGAL::Point_2<Kernel>* ipoint
               = CGAL::object_cast<CGAL::Point_2<Kernel> >(&result))
              {
                if((s->source() == *ipoint) || (s->target() == *ipoint))
                  {
                    logger->debug(msg("segment %1 and %2 share point (%3,%4)")
                                  .arg(to_string(*s))
                                  .arg(to_string(*t))
                                  .arg(CGAL::to_double(ipoint->x()))
                                  .arg(CGAL::to_double(ipoint->y())));
                  }
                else if(points.find(*ipoint) != points.end())
                  {
                    logger->debug(msg("segment %1 intersects %2 in input point"
                                      " (%3,%4)")
                                  .arg(to_string(*s))
                                  .arg(to_string(*t))
                                  .arg(CGAL::to_double(ipoint->x()))
                                  .arg(CGAL::to_double(ipoint->y())));
                  }
                else
                  {
                    logger->debug(msg("segment %1 intersects %2 in (%3,%4)")
                                  .arg(to_string(*s))
                                  .arg(to_string(*t))
                                  .arg(CGAL::to_double(ipoint->x()))
                                  .arg(CGAL::to_double(ipoint->y())));
                  }
              }
            else if (const CGAL::Segment_2<Kernel>* iseg
                     = CGAL::object_cast<CGAL::Segment_2<Kernel> >(&result))
              {
                if(*iseg == *s)
                  {
                    logger->debug(msg("segment %1 is invalid")
                                  .arg(to_string(*s)));
                  }
                else if(*iseg == *t)
                  {
                    logger->debug(msg("segment %1 is invalid")
                                  .arg(to_string(*t)));
                  }
                else
                  {
                    logger->debug(msg("segments %1 and %2 are invalid")
                                  .arg(to_string(*s))
                                  .arg(to_string(*t)));
                  }
              }
            else
              {
                    // handle the no intersection case.
              }
          }
      }
  }
};

#endif // MMT_INTERSECTION_NAIVE_INTERSECTION_ALGORITHM_H
