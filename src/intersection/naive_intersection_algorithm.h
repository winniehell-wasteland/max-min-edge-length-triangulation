#ifndef MMT_INTERSECTION_NAIVE_INTERSECTION_ALGORITHM_H
#define MMT_INTERSECTION_NAIVE_INTERSECTION_ALGORITHM_H

#include <CGAL/intersections.h>

#include "logger.h"

#include "intersection/intersection_graph.h"

class NaiveIntersectionAlgorithm
{
  typedef std::map<Point, IntersectionGroup, STLPointOrder>  Intersections;
  typedef std::set<SegmentIndex>                             Overlaps;
public:
  NaiveIntersectionAlgorithm(IntersectionGraph* graph,
                             const PointSet& points,
                             SegmentContainer& segments) :
    empty_group_(),
    intersections_(point_order)
  {
    CGAL_precondition_msg(graph != 0, "IntersectionGraph has to be initialized!");

    SegmentIndex overlaps = 0;

    for(SegmentContainer::iterator s = segments.begin();
        s != segments.end();
        ++s)
      {
        SegmentContainer::iterator t = s;
        for( ++t; t != segments.end(); ++t)
          {
            logger.debug(mmt_msg("check segment %1 and %2")
                         .arg(print_segment(*s))
                         .arg(print_segment(*t)));

            CGAL::Object result = CGAL::intersection(
              static_cast<CGAL::Segment_2<Kernel> >(*s),
              static_cast<CGAL::Segment_2<Kernel> >(*t)
            );

            if(const CGAL::Point_2<Kernel>* ipoint
                    = CGAL::object_cast<CGAL::Point_2<Kernel> >(&result))
            {
                if(points.find(*ipoint) != points.end())
                {
                    logger.debug(mmt_msg("segment %1 intersects %2"
                                     " in input point %3")
                                 .arg(print_segment(*s))
                                 .arg(print_segment(*t))
                                 .arg(print_point(*ipoint)));
                }
                else if((s->source() == *ipoint) || (s->target() == *ipoint))
                {
                    logger.debug(mmt_msg("segment %1 and %2 share point %3")
                                 .arg(print_segment(*s))
                                 .arg(print_segment(*t))
                                 .arg(print_point(*ipoint)));
                }
                else
                {
                    logger.debug(mmt_msg("segment %1 intersects %2 in %3")
                                 .arg(print_segment(*s))
                                 .arg(print_segment(*t))
                                 .arg(print_point(*ipoint)));

                    add_intersecting_segment(*ipoint, *s);
                    add_intersecting_segment(*ipoint, *t);
                }
              }
            else if (const CGAL::Segment_2<Kernel>* iseg
                     = CGAL::object_cast<CGAL::Segment_2<Kernel> >(&result))
              {
                CGAL_precondition(*iseg != *t);

                if(*iseg == *s)
                  {
                    logger.debug(mmt_msg("segment %1 contains %2")
                                 .arg(print_segment(*t))
                                 .arg(print_segment(*s)));

                    if(!s->data().overlapping)
                      {
                        s->data().overlapping = true;
                        ++overlaps;
                      }
                  }
                else
                  {
                    logger.debug(mmt_msg("segments %1 and %2 overlap")
                                 .arg(print_segment(*s))
                                 .arg(print_segment(*t)));
                  }
              }
            else
              {
                    // handle the no intersection case.
              }
          }
      }

    SegmentIndex intersecting_segments = 0;

    for(Intersections::const_iterator intersection = intersections_.begin();
        intersection != intersections_.end(); ++intersection)
      {
        logger.debug(mmt_msg("intersection group: %1")
                     .arg(print_igroup(segments, intersection->second)));

        intersecting_segments += intersection->second.size();
        graph->add_intersection_group(intersection->second);
      }

    logger.print(mmt_msg("intersections=%1 overlaps=%2")
                 .arg(intersecting_segments, 10, 10, QChar('0'))
                 .arg(overlaps, 3, 10, QChar('0')));
  }
private:
  IntersectionGroup empty_group_;
  Intersections intersections_;

  void add_intersecting_segment(const Point& point, const Segment& segment)
  {
    // insert key if it does not exist
    Intersections::iterator key =
        intersections_.insert(std::make_pair(point, empty_group_)).first;

    // add segment to group
    key->second.insert(segment.data().index);
  }
};

#endif // MMT_INTERSECTION_NAIVE_INTERSECTION_ALGORITHM_H
