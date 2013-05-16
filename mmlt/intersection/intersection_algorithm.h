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

        // loop over all segments into s1
        for(auto s1 = segments_.begin(); s1 != segments_.end(); ++s1)
        {
            bool segment_intersects = false;

            // loop over all segments after s1 into s2
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
                    // intersection is segment endpoint
                    if((s1->source() == *ipoint) || (s1->target() == *ipoint))
                    {
                        handle_same_endpoint(*ipoint, *s1, *s2);
                    }
                    // intersection is input point
                    else if(points_.contains(*ipoint))
                    {
                        handle_input_point(*ipoint, *s1, *s2);

                        segment_intersects = true;
                    }
                    // intersection lies on segment
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
                    // s2 must be the longer segment (segments are sorted by length)
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

            // found first non-intersecting segment
            if(!segment_intersects)
            {
                shortest_nonintersecting_segment = s1->data().index;
                logger.info(mmlt_msg("shortest non-intersecting segment: %1 (%2)")
                            .arg(s1->to_string())
                            .arg(segment_length_to_string(*s1)));

                break;
            }
        }

        logger.print(mmlt_msg("separators=%1").arg(separators_.size()));

        // loop over all separators into sep1_index
        for(auto sep1_index = separators_.begin(); sep1_index != separators_.end(); ++sep1_index)
        {
            Segment& sep1 = segments_[*sep1_index];

            // loop over all separators after sep1_index into sep2_index
            auto s2_index = sep1_index;
            for(++s2_index; s2_index != separators_.end(); ++s2_index)
            {
                Segment& sep2 = segments_[*s2_index];

                MMLT_precondition(segment_length_order(sep1, sep2) != CGAL::LARGER);

                logger.debug(mmlt_msg("test separators %1 and %2 for intersection")
                             .arg(sep1.to_string())
                             .arg(sep2.to_string()));

                CGAL::Object result = CGAL::intersection(
                            static_cast<CGAL::Segment_2<Kernel> >(sep1),
                            static_cast<CGAL::Segment_2<Kernel> >(sep2)
                );

                // intersection is a point
                if(const Point* ipoint = CGAL::object_cast<Point>(&result))
                {
                    // intersection is segment endpoint
                    if((sep1.source() == *ipoint) || (sep1.target() == *ipoint))
                    {
                        handle_same_endpoint(*ipoint, sep1, sep2);
                    }
                    // intersection is input point
                    else if(points_.contains(*ipoint))
                    {
                        handle_input_point(*ipoint, sep1, sep2);
                    }
                    // intersection lies on segment
                    else
                    {
                        handle_intersection(*ipoint, sep1, sep2);
                    }
                }
                // intersection is a segment
                else if(auto iseg = CGAL::object_cast<CGAL::Segment_2<Kernel> >(&result))
                {
                    // s2 must be the longer segment (segments are sorted by length)
                    MMLT_assertion(*iseg != sep2);

                    if(*iseg == sep1)
                    {
                        handle_inclusion(sep2, sep1);
                    }
                    // there is another inner segment
                    else
                    {
                        handle_overlap(sep1, sep2);
                    }
                }
            }
        }

        // add intersections to intersection graph
        for(auto& intersection : intersections_)
        {
            logger.debug(mmlt_msg("intersection group: %1")
                         .arg(intersection.second.to_string(segments_)));

            igraph.add_intersection_group(intersection.second);
        }
    }
private:
    /** empty intersection group for map insertion */
    IntersectionGroup       empty_group_;

    /** map of intersection points to segments */
    Intersections           intersections_;

    /** input points */
    const PointSet&         points_;

    /** input segments */
    SegmentContainer&       segments_;

    /** set of separators */
    std::set<SegmentIndex>  separators_;

    /**
     * store segment at intersection point
     */
    void add_intersecting_segment(const Point& intersection_point, const Segment& segment)
    {
        // insert key if it does not exist
        auto key = intersections_.insert(std::make_pair(intersection_point, empty_group_)).first;

        // add segment to group
        key->second.insert(segment.data().index);
    }

    /**
     * segment outer contains inner
     */
    void handle_inclusion(Segment& outer, const Segment& inner) const
    {
        logger.debug(mmlt_msg("segment %1 contains %2")
                     .arg(outer.to_string())
                     .arg(inner.to_string()));

        outer.data().overlapping = true;
    }

    /**
     * segments intersect in input point
     */
    void handle_input_point(const Point& input_point, const Segment& s1, const Segment& s2) const
    {
        logger.debug(mmlt_msg("segment %1 intersects %2"
                             " in input point %3")
                     .arg(s1.to_string())
                     .arg(s2.to_string())
                     .arg(input_point.to_string()));
    }

    /**
     * segments intersect
     */
    void handle_intersection(const Point& intersection_point, const Segment& s1, const Segment& s2)
    {
        logger.debug(mmlt_msg("segments %1 and %2 intersect in %3")
                     .arg(s1.to_string())
                     .arg(s2.to_string())
                     .arg(intersection_point.to_string()));

        add_intersecting_segment(intersection_point, s1);
        add_intersecting_segment(intersection_point, s2);
    }

    /**
     * segments overlap in more than one point
     */
    void handle_overlap(const Segment& s1, const Segment& s2) const
    {
        logger.debug(mmlt_msg("segments %1 and %2 overlap")
                     .arg(s1.to_string())
                     .arg(s2.to_string()));
    }

    /**
     * segments have the same end point
     */
    void handle_same_endpoint(const Point& end_point, const Segment& s1, const Segment& s2) const
    {
        logger.debug(mmlt_msg("segment %1 and %2 share point %3")
                     .arg(s1.to_string())
                     .arg(s2.to_string())
                     .arg(end_point.to_string()));
    }
};

#endif // MMLT_INTERSECTION_INTERSECTION_ALGORITHM_H
