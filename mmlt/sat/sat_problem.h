#pragma once
#ifndef MMLT_SAT_SAT_PROBLEM_H
#define MMLT_SAT_SAT_PROBLEM_H

#include "containers/segment_container.h"

#include "intersection/intersection_graph.h"
#include "intersection/intersections.h"

class SATProblem
{
public:
    /**
     * default constructor
     */
    SATProblem(const IntersectionGraph& igraph, const SegmentContainer& segments);

    const Intersections& intersections(const SegmentIndex& index) const
    {
        return igraph_[index];
    }

    const SegmentIndex& lower_bound() const
    {
        return lower_bound_;
    }

    const SegmentContainer& segments() const
    {
        return segments_;
    }

    /**
     * set range of segments to consider
     */
    void set_short_segment_range(const SegmentIndex& lower_bound,
                                 const SegmentIndex& upper_bound);

    SegmentIndex size() const
    {
        return segments_.size();
    }

    const SegmentIndex& upper_bound() const
    {
        return upper_bound_;
    }

protected:
    const IntersectionGraph&  igraph_;
    const SegmentContainer&   segments_;

    SegmentIndex lower_bound_;
    SegmentIndex upper_bound_;
};

#endif // MMLT_SAT_SAT_PROBLEM_H
