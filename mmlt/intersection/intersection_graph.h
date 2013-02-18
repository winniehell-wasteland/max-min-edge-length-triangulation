#pragma once
#ifndef MMLT_INTERSECTION_INTERSECTION_GRAPH_H
#define MMLT_INTERSECTION_INTERSECTION_GRAPH_H

#include <QPainter>

#include "config.h"
#include "containers/segment_container.h"
#include "intersection/intersection_group.h"

class IntersectionGraph
{
public:
    /** vector of @ref IntersectionGroup */
    using IntersectionGroupVector = std::vector<IntersectionGroup>;

    /** default constructor */
    IntersectionGraph(const PointSet& points, SegmentContainer& segments);

    void add_intersection_group(const IntersectionGroup& group);

    /** output intersection graph using QPainter */
    void draw(QPainter& painter) const;

    IntersectionGroupVector::const_iterator begin() const
    {
        return intersection_groups_.begin();
    }

    IntersectionGroupVector::const_iterator end() const
    {
        return intersection_groups_.end();
    }

    /** find the shortest segment that does not intersect with longer segments */
    SegmentIndex shortest_nonintersecting_segment() const;
private:
    IntersectionGroupVector  intersection_groups_;
    SegmentContainer&        segments_;
};

#endif // MMLT_INTERSECTION_INTERSECTION_GRAPH_H
