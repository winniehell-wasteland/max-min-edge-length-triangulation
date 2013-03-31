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
    SegmentIndex  shortest_nonintersecting_segment;

    IntersectionAlgorithm(const PointSet& points,
                          SegmentContainer& segments) :
        shortest_nonintersecting_segment(0),
        empty_group_(),
        intersections_(point_order),
        points_(points),
        segments_(segments),
        separators_()
    {
    }

    void run(IntersectionGraph& igraph)
    {
        intersections_.clear();
        separators_.clear();
        shortest_nonintersecting_segment = 0;

        for(auto s1 = segments_.begin(); s1 != segments_.end(); ++s1)
        {
            bool segment_intersects = false;

            auto s2 = s1;
            for(++s2; s2 != segments_.end(); ++s2)
            {
                MMLT_precondition(segment_length_order(*s1, *s2) != CGAL::LARGER);

                logger.debug(mmlt_msg("test segments %1 and %2 for intersection")
                             .arg(s1->to_string())
                             .arg(s2->to_string()));

                CGAL::Object result = CGAL::intersection(
                            static_cast<CGAL::Segment_2<Kernel> >(*s1),
                            static_cast<CGAL::Segment_2<Kernel> >(*s2)
                );

                // intersection is a point
                if(const Point* ipoint = CGAL::object_cast<Point>(&result))
                {
                    if((s1->source() == *ipoint) || (s1->target() == *ipoint))
                    {
                        handle_same_endpoint(*ipoint, *s1, *s2);
                    }
                    else if(points_.contains(*ipoint))
                    {
                        handle_input_point(*ipoint, *s1, *s2);

                        segment_intersects = true;
                    }
                    else
                    {
                        handle_intersection(*ipoint, *s1, *s2);
                        separators_.insert(s2->data().index);

                        segment_intersects = true;
                    }
                }
                // intersection is a segment
                else if(auto iseg = CGAL::object_cast<CGAL::Segment_2<Kernel> >(&result))
                {
                    // s2 must be the longer segment
                    MMLT_assertion(*iseg != *s2);

                    if(*iseg == *s1)
                    {
                        handle_inclusion(*s2, *s1);
                    }
                    // there is another inner segment
                    else
                    {
                        handle_overlap(*s1, *s2);
                    }

                    segment_intersects = true;
                }
            }

            if(!segment_intersects)
            {
                shortest_nonintersecting_segment = s1->data().index;
                logger.info(mmlt_msg("shortest non-intersecting segment: %1 (len^2=%2)")
                            .arg(s1->to_string())
                            .arg(CGAL::to_double(s1->squared_length())));

                break;
            }
            else {
                // just for testing!
                continue;
            }
        }

        //logger.print(mmlt_msg("separators=%1").arg(separators_.size()));

            /*
        for(auto s1_index = separators_.begin(); s1_index != separators_.end(); ++s1_index)
        {
            Segment& s1 = segments_[*s1_index];

            auto s2_index = s1_index;
            for(++s2_index; s2_index != separators_.end(); ++s2_index)
            {
                Segment& s2 = segments_[*s2_index];

                MMLT_precondition(segment_length_order(s1, s2) != CGAL::LARGER);

                logger.debug(mmlt_msg("test segments %1 and %2 for intersection")
                             .arg(s1.to_string())
                             .arg(s2.to_string()));

                CGAL::Object result = CGAL::intersection(
                            static_cast<CGAL::Segment_2<Kernel> >(s1),
                            static_cast<CGAL::Segment_2<Kernel> >(s2)
                );

                // intersection is a point
                if(const Point* ipoint = CGAL::object_cast<Point>(&result))
                {
                    if((s1.source() == *ipoint) || (s1.target() == *ipoint))
                    {
                        handle_same_endpoint(*ipoint, s1, s2);
                    }
                    else if(points_.contains(*ipoint))
                    {
                        handle_input_point(*ipoint, s1, s2);
                    }
                    else
                    {
                        handle_intersection(*ipoint, s1, s2);
                    }
                }
                // intersection is a segment
                else if(auto iseg = CGAL::object_cast<CGAL::Segment_2<Kernel> >(&result))
                {
                    // s2 must be the longer segment
                    MMLT_assertion(*iseg != s2);

                    if(*iseg == s1)
                    {
                        handle_inclusion(s2, s1);
                    }
                    // there is another inner segment
                    else
                    {
                        handle_overlap(s1, s2);
                    }
                }
            }
        }

        for(auto& intersection : intersections_)
        {
            logger.debug(mmlt_msg("intersection group: %1")
                         .arg(intersection.second.to_string(segments_)));

            igraph.add_intersection_group(intersection.second);
        }

        */
    }
private:
    IntersectionGroup       empty_group_;
    Intersections           intersections_;
    const PointSet&         points_;
    SegmentContainer&       segments_;
    std::set<SegmentIndex>  separators_;

    void add_intersecting_segment(const Point& intersection_point, const Segment& segment)
    {
        // insert key if it does not exist
        auto key = intersections_.insert(std::make_pair(intersection_point, empty_group_)).first;

        // add segment to group
        key->second.insert(segment.data().index);
    }

    void handle_inclusion(Segment& outer, const Segment& inner) const
    {
        logger.debug(mmlt_msg("segment %1 contains %2")
                     .arg(outer.to_string())
                     .arg(inner.to_string()));

        outer.data().overlapping = true;
    }

    void handle_input_point(const Point& input_point, const Segment& s1, const Segment& s2) const
    {
        logger.debug(mmlt_msg("segment %1 intersects %2"
                             " in input point %3")
                     .arg(s1.to_string())
                     .arg(s2.to_string())
                     .arg(input_point.to_string()));
    }

    void handle_intersection(const Point& intersection_point, const Segment& s1, const Segment& s2)
    {
        logger.debug(mmlt_msg("segments %1 and %2 intersect in %3")
                     .arg(s1.to_string())
                     .arg(s2.to_string())
                     .arg(intersection_point.to_string()));

        add_intersecting_segment(intersection_point, s1);
        add_intersecting_segment(intersection_point, s2);
    }

    void handle_overlap(const Segment& s1, const Segment& s2) const
    {
        logger.debug(mmlt_msg("segments %1 and %2 overlap")
                     .arg(s1.to_string())
                     .arg(s2.to_string()));
    }

    void handle_same_endpoint(const Point& end_point, const Segment& s1, const Segment& s2) const
    {
        logger.debug(mmlt_msg("segment %1 and %2 share point %3")
                     .arg(s1.to_string())
                     .arg(s2.to_string())
                     .arg(end_point.to_string()));
    }
};

#endif // MMLT_INTERSECTION_INTERSECTION_ALGORITHM_H
