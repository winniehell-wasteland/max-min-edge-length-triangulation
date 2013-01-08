#ifndef MMT_INTERSECTION_NAIVE_INTERSECTION_ALGORITHM_H
#define MMT_INTERSECTION_NAIVE_INTERSECTION_ALGORITHM_H

#include <CGAL/intersections.h>

#include "cgal_utils.h"

#include "intersection_graph.h"

#include <QDebug>

class NaiveIntersectionAlgorithm
{
  typedef std::map<Point, IntersectionGroup, STLPointOrder>  Intersections;
  typedef std::set<SegmentIndex>                             Overlaps;
public:
  NaiveIntersectionAlgorithm(IntersectionGraph* graph, const PointSet& points) :
    empty_group_(),
    intersections_(point_order)
  {
    CGAL_precondition_msg(graph != 0, "IntersectionGraph has to be initialized!");

    SegmentIndex overlaps = 0;

    for(SegmentVector::iterator s = graph->segments().begin();
        s != graph->segments().end(); ++s)
      {
        SegmentVector::iterator t = s;
        for( ++t; t != graph->segments().end(); ++t)
          {
            qDebug() << msg("check segment %1 and %2")
                        .arg(to_string(*s))
                        .arg(to_string(*t));

            CGAL::Object result = CGAL::intersection(
              static_cast<CGAL::Segment_2<Kernel> >(*s),
              static_cast<CGAL::Segment_2<Kernel> >(*t)
            );

            if(const CGAL::Point_2<Kernel>* ipoint
               = CGAL::object_cast<CGAL::Point_2<Kernel> >(&result))
              {
                if((s->source() == *ipoint) || (s->target() == *ipoint))
                  {
                    qDebug() << msg("segment %1 and %2 share point (%3,%4)")
                                .arg(to_string(*s))
                                .arg(to_string(*t))
                                .arg(CGAL::to_double(ipoint->x()))
                                .arg(CGAL::to_double(ipoint->y()));
                  }
                else if(points.find(*ipoint) != points.end())
                  {
                    qDebug() << msg("segment %1 intersects %2 in input point"
                                    " (%3,%4)")
                                .arg(to_string(*s))
                                .arg(to_string(*t))
                                .arg(CGAL::to_double(ipoint->x()))
                                .arg(CGAL::to_double(ipoint->y()));
                  }
                else
                  {
                    qDebug() << msg("segment %1 intersects %2 in (%3,%4)")
                                .arg(to_string(*s))
                                .arg(to_string(*t))
                                .arg(CGAL::to_double(ipoint->x()))
                                .arg(CGAL::to_double(ipoint->y()));

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
                    qDebug() << msg("segment %1 contains %2")
                                .arg(to_string(*t))
                                .arg(to_string(*s));

                    if(!s->data().overlapping)
                      {
                        s->data().overlapping = true;
                        ++overlaps;
                      }
                  }
                else
                  {
                    qDebug() << msg("segments %1 and %2 overlap")
                                .arg(to_string(*s))
                                .arg(to_string(*t));
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
        qDebug() << msg("intersection group:");

        for(IntersectionGroup::const_iterator segment_index = intersection->second.begin();
            segment_index != intersection->second.end(); ++segment_index)
          {
            ++intersecting_segments;
            qDebug() << to_string(graph->segments()[*segment_index]);
          }

        qDebug() << "";

        graph->add_intersection_group(intersection->second);
      }

    qWarning() << msg("segments: total=%1 intersections=%2 overlaps=%3")
                  .arg(graph->segments().size(), 5, 10, QChar('0'))
                  .arg(intersecting_segments, 10, 10, QChar('0'))
                  .arg(overlaps, 3, 10, QChar('0'));
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
