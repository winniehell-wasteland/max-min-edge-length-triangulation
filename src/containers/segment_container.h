#ifndef SEGMENT_CONTAINER_H
#define SEGMENT_CONTAINER_H

#include "config.h"

#include <QString>

class SegmentContainer :
        public std::vector<Segment>
{
public:
    SegmentContainer(const PointSet& points);
};

#endif // SEGMENT_CONTAINER_H
