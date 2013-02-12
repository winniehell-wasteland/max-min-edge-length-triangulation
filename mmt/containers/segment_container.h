#ifndef CONTAINERS__SEGMENT_CONTAINER_H
#define CONTAINERS__SEGMENT_CONTAINER_H

#include <vector>

#include "cgal/kernel.h"
#include "containers/types.h"

/** vector of segments */
typedef std::vector<Segment>                 SegmentVector;

class SegmentContainer :
        public SegmentVector
{
public:
    SegmentContainer(const PointSet& points);
};

#endif // CONTAINERS__SEGMENT_CONTAINER_H
