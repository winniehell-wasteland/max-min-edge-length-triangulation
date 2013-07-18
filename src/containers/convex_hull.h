#pragma once
#ifndef MMLT__CONTAINERS__CONVEX_HULL_H
#define MMLT__CONTAINERS__CONVEX_HULL_H

#include "containers/segment_container.h"

namespace MMLT
{

class ConvexHull :
        public std::vector<Point>
{
public:
    /** compute convex hull of given point set */
    ConvexHull(const PointSet& points);

    /** find the convex hull segment with minimum length */
    const SegmentIndex& shortest_segment(const SegmentContainer& segments) const;
};

}

#endif // MMLT__CONTAINERS__CONVEX_HULL_H
