#pragma once
#ifndef MMLT_INTERSECTION_INTERSECTION_GRAPH_H
#define MMLT_INTERSECTION_INTERSECTION_GRAPH_H

#include "config.h"

#include <QPainter>

#include "containers/segment_container.h"

#include "intersection/intersection_group.h"

#include "utils/assertions.h"

class IntersectionGraph
{
public:
    /** vector of @ref IntersectionGroup */
    using IntersectionGroupVector = std::vector<IntersectionGroup>;

    /** default constructor */
    IntersectionGraph(const PointSet& points, SegmentContainer& segments);

    const IntersectionGroup& operator[] (const IntersectionGroupIndex& index) const
    {
        MMLT_precondition(index < intersection_groups_.size());
        return intersection_groups_[index];
    }

    void add_intersection_group(const IntersectionGroup& group);

    IntersectionGroupVector::const_iterator begin() const
    {
        return intersection_groups_.begin();
    }

    /** output intersection graph using QPainter */
    void draw(QPainter& painter) const;

    IntersectionGroupVector::const_iterator end() const
    {
        return intersection_groups_.end();
    }

    SegmentIndex longest_intersecting_segment(const SegmentIndex& index) const;

    /** find the shortest segment that does not intersect with longer segments */
    SegmentIndex shortest_nonintersecting_segment() const;
private:
    IntersectionGroupVector  intersection_groups_;
    SegmentContainer&        segments_;
};

#endif // MMLT_INTERSECTION_INTERSECTION_GRAPH_H
