#ifndef MMT_INTERSECTION_NAIVE_INTERSECTION_ALGORITHM_H
#define MMT_INTERSECTION_NAIVE_INTERSECTION_ALGORITHM_H

#include <CGAL/intersections.h>

#include "utils.h"

#include "intersection_graph.h"

class NaiveIntersectionAlgorithm
{
  typedef std::map<Point, IntersectionGroup, STLPointOrder>  Intersections;
  typedef std::set<Segment, STLSegmentOrder>                 Overlaps;
public:
  NaiveIntersectionAlgorithm(IntersectionGraph* graph, const PointSet& points) :
    empty_group_(CGAL::compare_to_less(SegmentOrder())),
    intersections_(CGAL::compare_to_less(PointOrder())),
    overlaps_(CGAL::compare_to_less(SegmentOrder()))
  {
    CGAL_precondition_msg(graph != 0, "IntersectionGraph has to be initialized!");

    for(IntersectionGraph::SegmentIterator s = graph->segments_begin();
        s != graph->segments_end(); ++s)
      {
        IntersectionGraph::SegmentIterator t;
        for( t = s, ++t; t != graph->segments_end(); ++t)
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

                    add_intersecting_segment(*ipoint, *s);
                    add_intersecting_segment(*ipoint, *t);
                  }
              }
            else if (const CGAL::Segment_2<Kernel>* iseg
                     = CGAL::object_cast<CGAL::Segment_2<Kernel> >(&result))
              {
                if(*iseg == *s)
                  {
                    logger->debug(msg("segment %1 contains %2")
                                  .arg(to_string(*s))
                                  .arg(to_string(*t)));
                    overlaps_.insert(*s);
                  }
                else if(*iseg == *t)
                  {
                    logger->debug(msg("segment %1 contains %2")
                                  .arg(to_string(*t))
                                  .arg(to_string(*s)));
                    overlaps_.insert(*s);
                  }
                else
                  {
                    logger->debug(msg("segments %1 and %2 overlap")
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

    for(Intersections::const_iterator intersection = intersections_.begin();
        intersection != intersections_.end(); ++intersection)
      {
        logger->debug("intersection group:");

        for(IntersectionGroup::const_iterator segment = intersection->second.begin();
            segment != intersection->second.end(); ++segment)
          {
            logger->debug(to_string(*segment));
          }

        logger->debug("");

        graph->add_intersection_group(intersection->second);
      }

    for(Overlaps::const_iterator segment = overlaps_.begin();
        segment != overlaps_.end(); ++segment)
      {
        graph->remove_overlap(*segment);
      }
  }
private:
  IntersectionGroup empty_group_;
  Intersections intersections_;
  Overlaps overlaps_;

  void add_intersecting_segment(const Point& point, const Segment& segment)
  {
    // insert key if it does not exist
    Intersections::iterator key =
        intersections_.insert(std::make_pair(point, empty_group_)).first;

    // add segment to group
    key->second.insert(segment);
  }
};

#endif // MMT_INTERSECTION_NAIVE_INTERSECTION_ALGORITHM_H
