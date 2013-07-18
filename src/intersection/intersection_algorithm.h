#pragma once
#ifndef MMLT_INTERSECTION_INTERSECTION_ALGORITHM_H
#define MMLT_INTERSECTION_INTERSECTION_ALGORITHM_H

#include "cgal/kernel.h"

#include "containers/segment_container.h"

#include "intersection/intersection_graph.h"

namespace MMLT
{

class IntersectionAlgorithm
{
public:
    SegmentIndex  shortest_noncrossing_segment_;

    IntersectionAlgorithm();

    void run(IntersectionGraph& igraph, SegmentContainer& segments);
private:
    /**
     * segments cross
     */
    void handle_crossing(IntersectionGraph& igraph,
                         const Segment& s1,
                         const Segment& s2);

    /**
     * segments intersect but do not cross
     */
    void handle_overlap(IntersectionGraph& igraph,
                        const Segment& s1,
                        const Segment& s2);

    /**
     * segments have the same end point
     */
    void handle_same_endpoint(const Segment& s1,
                              const Segment& s2) const;

    /**
     * checks if two segments share an endpoint
     * @return the endpoint
     */
    bool have_same_endpoint(const Segment& s1,
                            const Segment& s2) const;

    /**
     * checks if two segments overlap
     * @return the outer segment
     */
    bool do_overlap(Segment& s1,
                    Segment& s2) const;
};

}

#endif // MMLT_INTERSECTION_INTERSECTION_ALGORITHM_H
