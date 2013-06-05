#include <CGAL/intersections.h>

#include "utils/assertions.h"
#include "utils/logger.h"

#include "intersection_algorithm.h"

IntersectionAlgorithm::IntersectionAlgorithm() :
    shortest_noncrossing_segment_(0)
{

}

void IntersectionAlgorithm::run(IntersectionGraph& igraph, SegmentContainer& segments)
{
    // set of separators
    std::set<SegmentIndex>  separators;

    // loop over all segments into s1
    for(auto s1 = segments.begin(); s1 != segments.end(); ++s1)
    {
        bool segment_intersects = false;

        // loop over all segments after s1 into s2
        auto s2 = s1;
        for(++s2; s2 != segments.end(); ++s2)
        {
            MMLT_precondition(segment_length_order(*s1, *s2) != CGAL::LARGER);

            /*
            logger.debug(mmlt_msg("test segments %1 and %2 for intersection")
                         .arg(s1->to_string())
                         .arg(s2->to_string()));
            */

            if(CGAL::do_intersect(*s1, *s2))
            {
                // segments share an enpoint
                if(have_same_endpoint(*s1, *s2))
                {
                    handle_same_endpoint(*s1, *s2);
                }
                // segments overlap
                else if(do_overlap(*s1, *s2))
                {
                    handle_overlap(igraph, *s1, *s2);
                }
                // segments cross
                else
                {
                    handle_crossing(igraph, *s1, *s2);

                    // store separator for second part of intersection graph
                    separators.insert(s2->data().index);

                    segment_intersects = true;
                }
            }
        }

        // found first non-crossing segment
        if(!segment_intersects)
        {
            shortest_noncrossing_segment_ = s1->data().index;
            logger.info(mmlt_msg("shortest non-crossing segment: %1 (%2)")
                        .arg(s1->to_string())
                        .arg(segment_length_to_string(*s1)));

            break;
        }
    }

    logger.print(mmlt_msg("separators=%1").arg(separators.size()));

    // loop over all separators into sep1_index
    for(auto sep1_index = separators.begin(); sep1_index != separators.end(); ++sep1_index)
    {
        Segment& sep1 = segments[*sep1_index];

        // loop over all separators after sep1_index into sep2_index
        auto s2_index = sep1_index;
        for(++s2_index; s2_index != separators.end(); ++s2_index)
        {
            Segment& sep2 = segments[*s2_index];

            MMLT_precondition(segment_length_order(sep1, sep2) != CGAL::LARGER);

            /*
            logger.debug(mmlt_msg("test separators %1 and %2 for intersection")
                         .arg(sep1.to_string())
                         .arg(sep2.to_string()));
            */

            if(CGAL::do_intersect(sep1, sep2))
            {
                // segments share an enpoint
                if(have_same_endpoint(sep1, sep2))
                {
                    handle_same_endpoint(sep1, sep2);
                }
                // segments overlap
                else if(do_overlap(sep1, sep2))
                {
                    handle_overlap(igraph, sep1, sep2);
                }
                // segments cross
                else
                {
                    handle_crossing(igraph, sep1, sep2);
                }
            }
        }
    }
}

void IntersectionAlgorithm::handle_crossing(IntersectionGraph& igraph,
                     const Segment& s1,
                     const Segment& s2)
{
    logger.debug(mmlt_msg("segments %1 and %2 intersect")
                 .arg(s1.to_string())
                 .arg(s2.to_string()));

    igraph.add_intersection(s1, s2);
}

void IntersectionAlgorithm::handle_overlap(IntersectionGraph& igraph,
                    const Segment& s1,
                    const Segment& s2)
{
    logger.info(mmlt_msg("segments %1 and %2 overlap")
                .arg(s1.to_string())
                .arg(s2.to_string()));

    igraph.add_intersection(s1, s2);
}

void IntersectionAlgorithm::handle_same_endpoint(const Segment& s1,
                          const Segment& s2) const
{
    logger.debug(mmlt_msg("segments %1 and %2 share an endpoint")
                 .arg(s1.to_string())
                 .arg(s2.to_string()));
}

bool IntersectionAlgorithm::have_same_endpoint(const Segment& s1,
                        const Segment& s2) const
{
    return (s1.source() == s2.source())
            || (s1.source() == s2.target())
            || (s1.target() == s2.source())
            || (s1.target() == s2.target());
}

bool IntersectionAlgorithm::do_overlap(Segment& s1,
                Segment& s2) const
{
    if(s1.has_on(s2.source()))
    {
        // check if s1 is the outer segment
        if(s1.has_on(s2.target()))
        {
            s1.data().is_outer = true;
        }

        return true;
    }
    else if(s1.has_on(s2.target()))
    {
        return true;
    }
    else if(s2.has_on(s1.source()) && s2.has_on(s1.target()))
    {
        // s2 is the outer segment
        s2.data().is_outer = true;
        return true;
    }

    return false;
}
