#ifndef MMT_INTERSECTION_GRAPH_H
#define MMT_INTERSECTION_GRAPH_H

#include "config.h"

#include <QPainter>

#include "parameters.h"

#include "containers/segment_container.h"

#include "intersection/types.h"

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

private:
    IntersectionGroupVector  intersection_groups_;
    SegmentContainer&        segments_;
};

#endif // MMT_INTERSECTION_GRAPH_H
