#ifndef SEGMENT_CONTAINER_H
#define SEGMENT_CONTAINER_H

#include "config.h"

#include <QString>

class SegmentContainer :
        public std::vector<Segment>
{
public:
    SegmentContainer(const PointSet& points);

    Segment& by_length(const SegmentIndex& index)
    {
        return at(by_length_[index]);
    }

    const Segment& by_length(const SegmentIndex& index) const
    {
        return at(by_length_[index]);
    }
private:
    std::vector<SegmentIndex> by_length_;
};

#endif // SEGMENT_CONTAINER_H
