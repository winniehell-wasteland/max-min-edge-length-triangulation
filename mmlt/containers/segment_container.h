#pragma once
#ifndef MMLT_CONTAINERS_SEGMENT_CONTAINER_H
#define MMLT_CONTAINERS_SEGMENT_CONTAINER_H

#include <boost/container/flat_set.hpp>

#include "cgal/kernel.h"
#include "containers/point_set.h"

/**
 * container of segments sorted by length
 */
class SegmentContainer :
        public boost::container::flat_multiset<Segment, STLSegmentLengthOrder>
{
public:
    SegmentContainer(const PointSet& points);

    Segment& operator[](const SegmentIndex& index);
    const Segment& operator[](const SegmentIndex& index) const;
};

#endif // MMLT_CONTAINERS_SEGMENT_CONTAINER_H
