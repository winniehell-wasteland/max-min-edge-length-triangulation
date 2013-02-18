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

    /** default constructor */
    IntersectionGraph(const PointSet& points, SegmentContainer& segments);

    void add_intersection_group(const IntersectionGroup& group);

    /** output intersection graph using QPainter */
    void draw_igraph(QPainter& painter) const;

    /** output intersection group using QPainter */
    void draw_igroup(QPainter& painter,
                     const IntersectionGroup& igroup) const;

    const IntersectionGroupVector& intersection_groups() const
    {
        return intersection_groups_;
    }

    /** find the shortest non-intersecting segment */
    SegmentIndex shortest_nonintersecting_segment() const;
private:
    IntersectionGroupVector  intersection_groups_;
    SegmentContainer&        segments_;
};

#endif // MMLT_INTERSECTION_INTERSECTION_GRAPH_H
