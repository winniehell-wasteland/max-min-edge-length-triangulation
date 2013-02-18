#pragma once
#ifndef MMLT_INTERSECTION_INTERSECTION_ALGORITHM_H
#define MMLT_INTERSECTION_INTERSECTION_ALGORITHM_H

#include <CGAL/intersections.h>

#include "utils/assertions.h"
#include "utils/logger.h"

#include "intersection/intersection_graph.h"

class IntersectionAlgorithm
{
    using Intersections = std::map<Point, IntersectionGroup, STLPointOrder>;
    using Overlaps      = std::set<SegmentIndex>;
public:
    IntersectionAlgorithm(IntersectionGraph* graph,
                          const PointSet& points,
                          SegmentContainer& segments) :
        empty_group_(),
        intersections_(point_order)
    {
        MMLT_precondition_msg(graph != 0, "IntersectionGraph has to be initialized!");

        SegmentIndex overlaps = 0;

        for(auto s = segments.begin(); s != segments.end(); ++s)
        {
            auto t = s;
            for( ++t; t != segments.end(); ++t)
            {
                logger.debug(mmlt_msg("check segment %1 and %2")
                             .arg(s->to_string())
                             .arg(t->to_string()));

                CGAL::Object result = CGAL::intersection(
                            static_cast<CGAL::Segment_2<Kernel> >(*s),
                            static_cast<CGAL::Segment_2<Kernel> >(*t)
                );

                if(const Point* ipoint = CGAL::object_cast<Point>(&result))
                {
                    if(points.find(*ipoint) != points.end())
                    {
                        logger.debug(mmlt_msg("segment %1 intersects %2"
                                             " in input point %3")
                                     .arg(s->to_string())
                                     .arg(t->to_string())
                                     .arg(ipoint->to_string()));
                    }
                    else if((s->source() == *ipoint) || (s->target() == *ipoint))
                    {
                        logger.debug(mmlt_msg("segment %1 and %2 share point %3")
                                     .arg(s->to_string())
                                     .arg(t->to_string())
                                     .arg(ipoint->to_string()));
                    }
                    else
                    {
                        logger.debug(mmlt_msg("segment %1 intersects %2 in %3")
                                     .arg(s->to_string())
                                     .arg(t->to_string())
                                     .arg(ipoint->to_string()));

                        add_intersecting_segment(*ipoint, *s);
                        add_intersecting_segment(*ipoint, *t);
                    }
                }
                else if (auto iseg = CGAL::object_cast<CGAL::Segment_2<Kernel> >(&result))
                {
                    if(*iseg == *s)
                    {
                        logger.debug(mmlt_msg("segment %1 contains %2")
                                     .arg(t->to_string())
                                     .arg(s->to_string()));

                        if(!s->data().overlapping)
                        {
                            s->data().overlapping = true;
                            ++overlaps;
                        }
                    }
                    else if(*iseg == *t)
                    {
                        logger.debug(mmlt_msg("segment %1 contains %2")
                                     .arg(s->to_string())
                                     .arg(t->to_string()));

                        if(!t->data().overlapping)
                        {
                            t->data().overlapping = true;
                            ++overlaps;
                        }
                    }
                    else
                    {
                        logger.debug(mmlt_msg("segments %1 and %2 overlap")
                                     .arg(s->to_string())
                                     .arg(t->to_string()));
                    }
                }
                else
                {
                    // handle the no intersection case.
                }
            }
        }

        SegmentIndex intersecting_segments = 0;

        for(auto& intersection : intersections_)
        {
            logger.debug(mmlt_msg("intersection group: %1")
                         .arg(intersection.second.to_string(segments)));

            intersecting_segments += intersection.second.size();
            graph->add_intersection_group(intersection.second);
        }

        logger.print(mmlt_msg("intersections=%1 overlaps=%2")
                     .arg(intersecting_segments, 10, 10, QChar('0'))
                     .arg(overlaps, 3, 10, QChar('0')));
    }
private:
    IntersectionGroup empty_group_;
    Intersections intersections_;

    void add_intersecting_segment(const Point& point, const Segment& segment)
    {
        // insert key if it does not exist
        auto key = intersections_.insert(std::make_pair(point, empty_group_)).first;

        // add segment to group
        key->second.insert(segment.data().index);
    }
};

#endif // MMLT_INTERSECTION_INTERSECTION_ALGORITHM_H
