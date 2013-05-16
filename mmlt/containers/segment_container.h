#pragma once
#ifndef MMLT_CONTAINERS_SEGMENT_CONTAINER_H
#define MMLT_CONTAINERS_SEGMENT_CONTAINER_H

#include <boost/container/flat_set.hpp>

#include <QPainter>

#include "cgal/kernel.h"

#include "containers/point_set.h"

/**
 * container of segments sorted by length
 */
class SegmentContainer :
        public boost::container::flat_multiset<Segment, STLSegmentLengthOrder>
{
public:
    /**
     * construct segments for all point pairs from set
     */
    SegmentContainer(const PointSet& points);

    /**
     * @name access i-th shortest segment
     * these operators assume that the segment set is not changed
     * after construction
     * @{
     */
    Segment& operator[](const SegmentIndex& index);
    const Segment& operator[](const SegmentIndex& index) const;
    /**
     * @}
     */

    /**
     * draws all segments
     */
    void draw(QPainter& painter) const;

    /**
     * draw a range of segments
     */
    void draw_range(QPainter& painter,
                    const SegmentIndex& lower_bound,
                    const SegmentIndex& upper_bound) const;
};

#endif // MMLT_CONTAINERS_SEGMENT_CONTAINER_H
