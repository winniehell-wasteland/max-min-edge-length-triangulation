#ifndef CONTAINERS__CONVEX_HULL_H
#define CONTAINERS__CONVEX_HULL_H

#include "containers/types.h"
#include "containers/segment_container.h"

class ConvexHull :
        public PointVector
{
public:
    ConvexHull(const PointSet& points);
    const SegmentIndex& shortest_segment(const SegmentContainer& segments) const;
};

#endif // CONTAINERS__CONVEX_HULL_H
