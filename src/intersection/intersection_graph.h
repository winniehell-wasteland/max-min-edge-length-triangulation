#pragma once
#ifndef MMLT_INTERSECTION_INTERSECTION_GRAPH_H
#define MMLT_INTERSECTION_INTERSECTION_GRAPH_H

#include "config.h"

#include <QPainter>

#include "intersection/intersections.h"

#include "utils/assertions.h"

namespace MMLT
{

class IntersectionGraph
{
public:
    /** vector of @ref Intersections */
    using IntersectionsVector = std::vector<Intersections>;

    /** default constructor */
    IntersectionGraph(const SegmentIndex& size);

    /**
     * @return all intersecting segments for a segment
     */
    const Intersections& operator[] (const SegmentIndex& index) const
    {
        MMLT_precondition(index < intersections_.size());
        return intersections_[index];
    }

    /**
     * add two intersecting segments to the graph
     */
    void add_intersection(const Segment &s1, const Segment &s2);

    IntersectionsVector::const_iterator begin() const
    {
        return intersections_.begin();
    }

    IntersectionsVector::const_iterator end() const
    {
        return intersections_.end();
    }

    const SegmentIndex& longest_intersecting_segment(const SegmentIndex& index) const;
private:
    IntersectionsVector  intersections_;
};

}

#endif // MMLT_INTERSECTION_INTERSECTION_GRAPH_H
